// Copyright (c) 2023-2024 Sophie Katz
//
// This file is part of Forge.
//
// Forge is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with Forge.
// If not, see <https://www.gnu.org/licenses/>.

#include "generators.hpp"
#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/LegacyPassManager.h>

struct frg_codegen_module_t {
    llvm::LLVMContext *llvm_ctx;
    llvm::IRBuilder<> *llvm_builder;
    llvm::Module* llvm_module;
};

extern "C" {
#include <forge/codegen/codegen.h>
#include <forge/assert.h>
#include <forge/memory.h>
#include <forge/messages/codes.h>

void frg_codegen_module_destroy(frg_codegen_module_t* module) {
    frg_assert_pointer_non_null(module);

    delete module->llvm_module;
    delete module->llvm_builder;
    delete module->llvm_ctx;

    frg_free(module);
}

void frg_codegen_module_print(
    frg_stream_output_t* mut_stream,
    const frg_codegen_module_t* module
) {
    frg_assert_pointer_non_null(mut_stream);
    frg_assert_pointer_non_null(module);

    std::string llvm_ir_string;
    llvm::raw_string_ostream llvm_stream(llvm_ir_string);

    module->llvm_module->print(llvm_stream, nullptr);

    frg_stream_output_write_string(
        mut_stream,
        llvm_ir_string.c_str()
    );
}

bool frg_codegen_module_write_object_file(
    frg_message_buffer_t* mut_message_buffer,
    const frg_codegen_module_t* module,
    const char* path
) {
    std::string target_triple = llvm::sys::getDefaultTargetTriple();

    frg_message_emit(
        mut_message_buffer,
        FRG_MESSAGE_SEVERITY_DEBUG,
        NULL,
        "Using target triple: %s",
        target_triple.c_str()
    );

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string error_message;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(
        target_triple,
        error_message
    );

    if (target == NULL) {
        frg_message_emit_fg_2_invalid_target_triple(
            mut_message_buffer,
            target_triple.c_str(),
            error_message.c_str()
        );

        return false;
    }

    llvm::TargetOptions target_options;
    llvm::TargetMachine* target_machine = target->createTargetMachine(
        target_triple,
        "generic",
        "",
        target_options,
        llvm::Reloc::PIC_
    );

    module->llvm_module->setDataLayout(target_machine->createDataLayout());
    module->llvm_module->setTargetTriple(target_triple);

    std::error_code error_code;
    llvm::raw_fd_ostream stream(
        path,
        error_code,
        llvm::sys::fs::OF_None
    );

    if (error_code) {
        frg_message_emit_ffs_1_open_for_writing(
            mut_message_buffer,
            path,
            error_code.message().c_str()
        );

        delete target_machine;

        return false;
    }

    llvm::legacy::PassManager pass;
    if (target_machine->addPassesToEmitFile(
        pass,
        stream,
        nullptr,
        llvm::CodeGenFileType::CGFT_ObjectFile
    )) {
        frg_message_emit_fg_1_cannot_emit_object_file(
            mut_message_buffer,
            target_triple.c_str()
        );

        delete target_machine;

        return false;
    }

    pass.run(*module->llvm_module);
    stream.flush();

    delete target_machine;

    return true;
}

frg_codegen_module_t* frg_codegen(const frg_ast_node_t* node) {
    frg_assert_pointer_non_null(node);

    frg_codegen_module_t* module = (frg_codegen_module_t*)frg_malloc(
        sizeof(frg_codegen_module_t)
    );

    module->llvm_ctx = new llvm::LLVMContext();
    module->llvm_builder = new llvm::IRBuilder<>(*module->llvm_ctx);
    module->llvm_module = new llvm::Module("forge", *module->llvm_ctx);

    if (node->kind != FRG_AST_NODE_KIND_DECLARATION_BLOCK) {
        frg_die("can only generate machine code from a declaration block");
    }

    const frg_ast_node_declaration_block_t* declaration_block = (const frg_ast_node_declaration_block_t*)node;

    frg_ast_scope_t* scope = frg_ast_scope_new();

    frg_ast_scope_load_declaration_block(
        scope,
        declaration_block
    );

    for (const GList* it = declaration_block->declarations; it != NULL; it = it->next) {
        const frg_ast_node_t* declaration = (const frg_ast_node_t*)it->data;

        switch (declaration->kind) {
            case FRG_AST_NODE_KIND_DECLARATION_FUNCTION:
                _frg_codegen_generate_declaration_function(
                    *module->llvm_builder,
                    *module->llvm_ctx,
                    *module->llvm_module,
                    scope,
                    declaration
                );
                break;
            default:
                frg_die_unexpected_enum_value(declaration->kind);
        }
    }

    frg_ast_scope_destroy(scope);

    return module;
}
}
