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

struct frg_llvm_module_t {
    llvm::LLVMContext *llvm_ctx;
    llvm::IRBuilder<> *llvm_builder;
    llvm::Module* llvm_module;
};

extern "C" {
#include <forge/codegen/codegen.h>
#include <forge/common/error.h>
#include <forge/common/memory.h>
#include <forge/messages/codes.h>

frg_llvm_module_t* frg_codegen(const frg_ast_t* ast) {
    frg_assert_pointer_non_null(ast);

    frg_llvm_module_t* llvm_module = (frg_llvm_module_t*)frg_safe_malloc(
        sizeof(frg_llvm_module_t)
    );

    llvm_module->llvm_ctx = new llvm::LLVMContext();
    llvm_module->llvm_builder = new llvm::IRBuilder<>(*llvm_module->llvm_ctx);
    llvm_module->llvm_module = new llvm::Module("forge", *llvm_module->llvm_ctx);

    frg_ast_decl_block_t* decl_block = frg_ast_try_cast_decl_block((frg_ast_t*)ast);
    if (decl_block == NULL) {
        frg_die("can only generate machine code from a declaration block");
    }

    frg_ast_scope_t* scope = frg_ast_scope_new();

    frg_ast_scope_load_decl_block(
        scope,
        decl_block
    );

    for (GList* it = decl_block->decls; it != NULL; it = it->next) {
        frg_ast_t* decl = (frg_ast_t*)it->data;

        switch (decl->id) {
            case FRG_AST_ID_DECL_FN:
                _frg_generate_decl_fn(
                    *llvm_module->llvm_builder,
                    *llvm_module->llvm_ctx,
                    *llvm_module->llvm_module,
                    scope,
                    decl
                );
                break;
            default:
                frg_die_unexpected_enum_value(decl->id);
        }
    }

    return llvm_module;
}

bool frg_codegen_write_object_file(
    frg_message_buffer_t* message_buffer,
    const frg_llvm_module_t* llvm_module,
    const char* path
) {
    std::string target_triple = llvm::sys::getDefaultTargetTriple();

    frg_message_emit(
        message_buffer,
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
            message_buffer,
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

    llvm_module->llvm_module->setDataLayout(target_machine->createDataLayout());
    llvm_module->llvm_module->setTargetTriple(target_triple);

    std::error_code error_code;
    llvm::raw_fd_ostream stream(
        path,
        error_code,
        llvm::sys::fs::OF_None
    );

    if (error_code) {
        frg_message_emit_ff_1_open_for_writing(
            message_buffer,
            path,
            error_code.message().c_str()
        );

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
            message_buffer,
            target_triple.c_str()
        );

        return false;
    }

    pass.run(*llvm_module->llvm_module);
    stream.flush();

    return true;
}

void frg_codegen_destroy_module(frg_llvm_module_t** llvm_module) {
    frg_assert_pointer_non_null(llvm_module);
    frg_assert_pointer_non_null(*llvm_module);

    delete (*llvm_module)->llvm_module;
    delete (*llvm_module)->llvm_builder;
    delete (*llvm_module)->llvm_ctx;

    frg_safe_free((void**) llvm_module);
}

void frg_codegen_print_module(
    frg_stream_output_t* stream,
    const frg_llvm_module_t* llvm_module
) {
    frg_assert_pointer_non_null(stream);
    frg_assert_pointer_non_null(llvm_module);

    std::string llvm_ir_string;
    llvm::raw_string_ostream llvm_stream(llvm_ir_string);

    llvm_module->llvm_module->print(llvm_stream, nullptr);

    frg_stream_output_write_string(
        stream,
        llvm_ir_string.c_str()
    );
}
}
