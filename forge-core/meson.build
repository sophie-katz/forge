# Copyright (c) 2023-2024 Sophie Katz
#
# This file is part of Forge.
#
# Forge is free software: you can redistribute it and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with Forge. If
# not, see <https://www.gnu.org/licenses/>.

# Project definition
# --------------------------------------------------------------------------------------
project(
    'forge-core',
    'c',
    'cpp',
    default_options: [
        'c_std=gnu18',
        'cpp_std=c++20',
        'werror=true'
    ],
    license: 'LGPL-3.0-only',
    meson_version: '>=1.1',
    version: '0.1.0'
)

# Options
# --------------------------------------------------------------------------------------

should_build_tests = get_option('test_mode') != 'disabled'

if get_option('test_mode') == 'fast'
    test_wrapper = '../scripts/test_wrapper_fast.bash'
    test_is_parallel = true
elif get_option('test_mode') == 'full'
    test_wrapper = '../scripts/test_wrapper_full.bash'
    test_is_parallel = false
endif

# Constants
# --------------------------------------------------------------------------------------

test_timeout_seconds = 120

# C/C++ global arguments
# --------------------------------------------------------------------------------------

# This is to temporarily fix a bug with Valgrind and recent versions of Clang.
#
# https://github.com/llvm/llvm-project/issues/56550
#
# Although it is fixed in Valgrind 3.20, this version has not get been packaged in
# Debian Bookworm (as of 2024-02-11).

add_global_arguments(
    '-gdwarf-4',
    language: ['c', 'cpp']
)

add_global_link_arguments(
    '-gdwarf-4',
    language: ['c', 'cpp']
)

# External programs
# --------------------------------------------------------------------------------------

program_bash = find_program(
    'bash',
    required: true,
)

program_flex = find_program(
    'flex',
    required: true,
    version: ['>=2.6.4', '<2.7']
)

program_bison = find_program(
    'bison',
    required: true,
    version: ['>=3.8.2', '<3.9']
)

program_llvm_config = find_program(
    'llvm-config',
    required: true,
    version: ['>=17.0.6', '<17.1']
)

message('LLVM version: ' + program_llvm_config.version())

# Generators
# --------------------------------------------------------------------------------------

generator_flex = generator(
    program_flex,
    output: '@BASENAME@.yy.c',
    arguments: ['-o', '@OUTPUT@', '@INPUT@']
)

generator_bison = generator(
    program_bison,
    output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
    arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@']
)

# Dependencies
# --------------------------------------------------------------------------------------

compiler_c = meson.get_compiler('c')
cmake = import('cmake')

# Subprojects
if should_build_tests
    subproject_unity = subproject(
        'Unity',
        required: true,
        default_options: [
            'buildtype=release',
        ]
    )
endif

subproject_utf8proc = cmake.subproject('utf8proc')

# Dependencies
if should_build_tests
    dependency_unity = subproject_unity.get_variable('unity_dep')
endif

dependency_utf8proc = subproject_utf8proc.dependency('utf8proc')
dependency_glib2_0 = dependency('glib-2.0')
dependency_m = compiler_c.find_library('m', required: true)

dependencies = [
    dependency_utf8proc,
    dependency_glib2_0,
    dependency_m
]

# LLVM
cppflags_llvm_config = run_command(
    [
        program_llvm_config,
        '--cppflags'
    ],
    capture: true,
    check: true,
    
)

add_project_arguments(
    cppflags_llvm_config.stdout().strip().split(' '),
    language: ['cpp']
)

ldflags_llvm_config = run_command(
    [
        program_llvm_config,
        '--ldflags'
    ],
    capture: true,
    check: true
)

libs_llvm_config = run_command(
    [
        program_llvm_config,
        '--libs'
    ],
    capture: true,
    check: true
)

add_project_link_arguments(
    ldflags_llvm_config.stdout().strip().split(' ')
        + libs_llvm_config.stdout().strip().split(' '),
    language: ['c', 'cpp']
)

# Build configuration
# --------------------------------------------------------------------------------------

git_commit_sha_result = run_command(
    [
        'git',
        'rev-parse',
        'HEAD'
    ],
    capture: true,
    check: true
)

build_configuration_data = configuration_data()
build_configuration_data.set('version_major', '0')
build_configuration_data.set('version_minor', '1')
build_configuration_data.set('version_patch', '0')
build_configuration_data.set('version_label', 'alpha')
build_configuration_data.set('git_commit_sha', git_commit_sha_result.stdout().strip())
build_configuration_data.set('compiler_id', compiler_c.get_id())
build_configuration_data.set('compiler_version', compiler_c.version())
build_configuration_data.set('linker_id', compiler_c.get_linker_id())
build_configuration_data.set('host_machine_cpu', host_machine.cpu())
build_configuration_data.set('host_machine_system', host_machine.system())

llvm_version = program_llvm_config.version().split('.')

build_configuration_data.set('llvm_version_major', llvm_version[0])
build_configuration_data.set('llvm_version_minor', llvm_version[1])
build_configuration_data.set('llvm_version_patch', llvm_version[2])

if should_build_tests
    build_configuration_data.set('should_build_tests', '1')
else
    build_configuration_data.set('should_build_tests', '0')
endif

if get_option('debug')
    build_configuration_data.set('should_build_asserts', '1')
    build_configuration_data.set('always_print_internal_message_prefix', '1')
else
    build_configuration_data.set('should_build_asserts', '0')
    build_configuration_data.set('always_print_internal_message_prefix', '0')
endif

configure_file(
    input: 'build_configuration.h.in',
    output: 'build_configuration.h',
    configuration: build_configuration_data
)

# Generated files
# --------------------------------------------------------------------------------------

generated_files_flex = generator_flex.process('lib/forge/parsing/forge.l')

generated_files_bison = generator_bison.process('lib/forge/parsing/forge.y')

# Forge libraries
# --------------------------------------------------------------------------------------
include_directories = include_directories('include')

library_forge = static_library(
    'forge',
    'lib/forge/ast/node.c',
    'lib/forge/ast/node_kind_info.c',
    'lib/forge/ast/utilities.c',
    'lib/forge/ast/cloners.c',
    'lib/forge/ast/debug_printers.c',
    'lib/forge/ast/comparers.c',
    'lib/forge/ast/print_debug.c',
    'lib/forge/ast/declaration_name_getters.c',
    'lib/forge/ast/destructors.c',
    'lib/forge/ast/scope.c',
    'lib/forge/ast/visitor.c',
    'lib/forge/ast/visitor_acceptors.c',
    'lib/forge/cli/choice.c',
    'lib/forge/cli/command.c',
    'lib/forge/cli/option_set.c',
    'lib/forge/cli/option.c',
    'lib/forge/cli/program.c',
    'lib/forge/codegen/codegen.cpp',
    'lib/forge/codegen/generators.cpp',
    'lib/forge/debug.c',
    'lib/forge/enum_printing.c',
    'lib/forge/enums.c',
    'lib/forge/lexical_casts/utilities.c',
    'lib/forge/lexical_casts/character.c',
    'lib/forge/lexical_casts/uint.c',
    'lib/forge/lexical_casts/float.c',
    'lib/forge/lexical_casts/string.c',
    'lib/forge/log.c',
    'lib/forge/math.c',
    'lib/forge/memory.c',
    'lib/forge/configuration/cli_banner.c',
    'lib/forge/configuration/cli_program.c',
    'lib/forge/configuration/commands/utilities.c',
    'lib/forge/configuration/commands/compile.c',
    'lib/forge/configuration/commands/help.c',
    'lib/forge/configuration/commands/link.c',
    'lib/forge/configuration/commands/version.c',
    'lib/forge/configuration/options.c',
    'lib/forge/formatting/formatted_printers.c',
    'lib/forge/formatting/print.c',
    'lib/forge/linking/configuration.c',
    'lib/forge/linking/link.c',
    'lib/forge/messages/message.c',
    'lib/forge/messages/message_buffer.c',
    'lib/forge/messages/query.c',
    'lib/forge/parsing/domain.c',
    'lib/forge/parsing/parse.c',
    'lib/forge/parsing/source.c',
    'lib/forge/parsing/source_context.c',
    'lib/forge/parsing/token_reader.c',
    'lib/forge/streams/input.c',
    'lib/forge/streams/output.c',
    'lib/forge/types.c',
    'lib/forge/spawn.c',
    'lib/forge/verification/visitor/supported_language_features.c',
    'lib/forge/verification/visitor/type_verification.c',
    'lib/forge/verification/type_resolvers.c',
    'lib/forge/verification/type_operations.c',
    'lib/forge/verification/verifier.c',
    'lib/forge/verification/visitor/well_formed.c',
    generated_files_flex,
    generated_files_bison,
    include_directories: include_directories,
    dependencies: dependencies,
    implicit_include_directories: true
)

library_forge_testing = static_library(
    'forge-testing',
    'lib/forge-testing/string_comparison.c',
    'lib/forge-testing/string_substitution.c',
    'lib/forge-testing/compilation_test/phases/initialization.c',
    'lib/forge-testing/compilation_test/options.c',
    'lib/forge-testing/compilation_test/phases/parsing.c',
    'lib/forge-testing/compilation_test/phases/linking.c',
    'lib/forge-testing/compilation_test/auxiliary/ast.c',
    'lib/forge-testing/compilation_test/auxiliary/verification.c',
    'lib/forge-testing/compilation_test/auxiliary/llvm_ir.c',
    'lib/forge-testing/compilation_test/auxiliary/shared_object.c',
    'lib/forge-testing/compilation_test/utilities.c',
    'lib/forge-testing/compilation_test/phases/codegen.c',
    'lib/forge-testing/compilation_test/test.c',
    include_directories: include_directories,
    dependencies: dependencies + [
        dependency_unity
    ],
    link_with: library_forge,
    implicit_include_directories: true
)

# Forge Executables
# --------------------------------------------------------------------------------------

executable(
    'forge',
    'src/forge/main.c',
    include_directories: include_directories,
    link_with: library_forge,
    dependencies: dependencies
)

# Tests
# --------------------------------------------------------------------------------------

if should_build_tests
    # Unit
    test_unit_forge_ast_scope = executable(
        'test-unit-forge-ast-scope',
        'lib/forge/ast/scope_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/ast/scope_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_ast_scope
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_ast_debug_printers = executable(
        'test-unit-forge-ast-debug-printers',
        'lib/forge/ast/debug_printers_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/ast/debug_printers_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_ast_debug_printers
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_ast_comparers = executable(
        'test-unit-forge-ast-comparers',
        'lib/forge/ast/comparers_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/ast/comparers_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_ast_comparers
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_ast_visitor = executable(
        'test-unit-forge-ast-visitor',
        'lib/forge/ast/visitor_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/ast/visitor_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_ast_visitor
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_memory = executable(
        'test-unit-forge-memory',
        'lib/forge/memory_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/memory_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_memory
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_lexical_casts_character = executable(
        'test-unit-forge-lexical-casts-character',
        'lib/forge/lexical_casts/character_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/lexical_casts/character_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_lexical_casts_character
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_lexical_casts_string = executable(
        'test-unit-forge-lexical-casts-string',
        'lib/forge/lexical_casts/string_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/lexical_casts/string_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_lexical_casts_string
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_lexical_casts_uint = executable(
        'test-unit-forge-lexical-casts-uint',
        'lib/forge/lexical_casts/uint_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/lexical_casts/uint_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_lexical_casts_uint
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_lexical_casts_utilities = executable(
        'test-unit-forge-lexical-casts-utilities',
        'lib/forge/lexical_casts/utilities_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/lexical_casts/utilities_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_lexical_casts_utilities
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_math = executable(
        'test-unit-forge-math',
        'lib/forge/math_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/math_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_math
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_enum_printing = executable(
        'test-unit-forge-enum-printing',
        'lib/forge/enum_printing_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/enum_printing_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_enum_printing
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_cli_option = executable(
        'test-unit-forge-cli-option',
        'lib/forge/cli/option_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/cli/option_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_cli_option
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_cli_option_set = executable(
        'test-unit-forge-cli-option-set',
        'lib/forge/cli/option_set_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/cli/option_set_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_cli_option_set
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_cli_program = executable(
        'test-unit-forge-cli-program',
        'lib/forge/cli/program_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/cli/program_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_cli_program
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_parsing_token_reader = executable(
        'test-unit-forge-parsing-token-reader',
        'lib/forge/parsing/token_reader_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/parsing/token_reader_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_parsing_token_reader
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_messages_message_buffer = executable(
        'test-unit-forge-messages-message-buffer',
        'lib/forge/messages/message_buffer_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/messages/message_buffer_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_messages_message_buffer
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_messages_message = executable(
        'test-unit-forge-messages-message',
        'lib/forge/messages/message_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/messages/message_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_messages_message
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_parsing_source = executable(
        'test-unit-forge-parsing-source',
        'lib/forge/parsing/source_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/parsing/source_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_parsing_source
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_parsing_parse = executable(
        'test-unit-forge-parsing-parse',
        'lib/forge/parsing/parse_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/parsing/parse_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_parsing_parse
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_streams_input = executable(
        'test-unit-forge-streams-input',
        'lib/forge/streams/input_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/streams/input_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_streams_input
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_streams_output = executable(
        'test-unit-forge-streams-output',
        'lib/forge/streams/output_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/streams/output_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_streams_output
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_verification_type_operations = executable(
        'test-unit-forge-verification-type-operations',
        'lib/forge/verification/type_operations_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/verification/type_operations_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_verification_type_operations
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_verification_visitor_well_formed = executable(
        'test-unit-forge-verification-visitor-well-formed',
        'lib/forge/verification/visitor/well_formed_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/verification/visitor/well_formed_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_verification_visitor_well_formed
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_enums = executable(
        'test-unit-forge-enums',
        'lib/forge/enums_test.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge/enums_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_enums
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_testing_string_comparison = executable(
        'test-unit-forge-testing-string-comparison',
        'lib/forge-testing/string_comparison_test.c',
        include_directories: include_directories,
        link_with: [library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge-testing/string_comparison_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_testing_string_comparison
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_testing_string_substitution = executable(
        'test-unit-forge-testing-string-substitution',
        'lib/forge-testing/string_substitution_test.c',
        include_directories: include_directories,
        link_with: [library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge-testing/string_substitution_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_testing_string_substitution
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_testing_compilation_test_auxiliary_ast = executable(
        'test-unit-forge-testing-compilation-test-axuiliary-ast',
        'lib/forge-testing/compilation_test/auxiliary/ast_test.c',
        include_directories: include_directories,
        link_with: [library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge-testing/compilation_test/auxiliary/ast_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_testing_compilation_test_auxiliary_ast
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_testing_compilation_test_auxiliary_llvm_ir = executable(
        'test-unit-forge-testing-compilation-test-axuiliary-llvm-ir',
        'lib/forge-testing/compilation_test/auxiliary/llvm_ir_test.c',
        include_directories: include_directories,
        link_with: [library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge-testing/compilation_test/auxiliary/llvm_ir_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_testing_compilation_test_auxiliary_llvm_ir
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_unit_forge_testing_compilation_test_phases_initialization = executable(
        'test-unit-forge-testing-compilation-test-phases-initialization',
        'lib/forge-testing/compilation_test/phases/initialization_test.c',
        include_directories: include_directories,
        link_with: [library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'lib/forge-testing/compilation_test/phases/initialization_test.c',
        program_bash,
        args: [
            test_wrapper,
            test_unit_forge_testing_compilation_test_phases_initialization
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    # Integration

    test_integration_compilation_empty = executable(
        'test-integration-compilation-empty',
        'tests/compilation/empty.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/empty.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_empty
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_literals_int = executable(
        'test-integration-compilation-literals-int',
        'tests/compilation/literals_int.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/literals_int.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_literals_int
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_literals_float = executable(
        'test-integration-compilation-literals-float',
        'tests/compilation/literals_float.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/literals_float.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_literals_float
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_literals_bool = executable(
        'test-integration-compilation-literals-bool',
        'tests/compilation/literals_bool.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/literals_bool.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_literals_bool
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_function_return_type = executable(
        'test-integration-compilation-function-return-type',
        'tests/compilation/function_return_type.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/function_return_type.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_function_return_type
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_bit_not = executable(
        'test-integration-compilation-bit-not',
        'tests/compilation/bit_not.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/bit_not.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_bit_not
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_bit_and = executable(
        'test-integration-compilation-bit-and',
        'tests/compilation/bit_and.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/bit_and.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_bit_and
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_bit_or = executable(
        'test-integration-compilation-bit-or',
        'tests/compilation/bit_or.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/bit_or.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_bit_or
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_bit_xor = executable(
        'test-integration-compilation-bit-xor',
        'tests/compilation/bit_xor.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/bit_xor.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_bit_xor
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_bit_shift_left = executable(
        'test-integration-compilation-bit-shift-left',
        'tests/compilation/bit_shift_left.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/bit_shift_left.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_bit_shift_left
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_bit_shift_right = executable(
        'test-integration-compilation-bit-shift-right',
        'tests/compilation/bit_shift_right.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/bit_shift_right.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_bit_shift_right
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_logical_not = executable(
        'test-integration-compilation-logical-not',
        'tests/compilation/logical_not.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/logical_not.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_logical_not
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_logical_and = executable(
        'test-integration-compilation-logical-and',
        'tests/compilation/logical_and.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/logical_and.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_logical_and
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_logical_or = executable(
        'test-integration-compilation-logical-or',
        'tests/compilation/logical_or.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/logical_or.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_logical_or
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_compilation_negate_int = executable(
        'test-integration-compilation-negate-int',
        'tests/compilation/negate_int.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/compilation/negate_int.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_compilation_negate_int
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    test_integration_nodes = executable(
        'test-integration-nodes',
        'tests/nodes.c',
        include_directories: include_directories,
        link_with: [library_forge, library_forge_testing],
        dependencies: dependencies + [
            dependency_unity
        ]
    )

    test(
        'tests/nodes.c',
        program_bash,
        args: [
            test_wrapper,
            test_integration_nodes
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )
endif

# Demos
# --------------------------------------------------------------------------------------

if should_build_tests
    demo_forge_streams_color = executable(
        'demo-forge-streams-color',
        'lib/forge/streams/color_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/streams/color_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_streams_color
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_streams_unicode = executable(
        'demo-forge-streams-unicode',
        'lib/forge/streams/unicode_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/streams/unicode_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_streams_unicode
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_debug = executable(
        'demo-forge-debug',
        'lib/forge/debug_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/debug_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_debug
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_cli_choice = executable(
        'demo-forge-cli-choice',
        'lib/forge/cli/choice_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/cli/choice_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_cli_choice
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_cli_option = executable(
        'demo-forge-cli-option',
        'lib/forge/cli/option_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/cli/option_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_cli_option
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_cli_option_set = executable(
        'demo-forge-cli-option-set',
        'lib/forge/cli/option_set_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/cli/option_set_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_cli_option_set
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_cli_command = executable(
        'demo-forge-cli-command',
        'lib/forge/cli/command_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/cli/command_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_cli_command
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_cli_program = executable(
        'demo-forge-cli-program',
        'lib/forge/cli/program_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/cli/program_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_cli_program
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_messages_sourced_messages = executable(
        'demo-forge-messages-sourced-messages',
        'lib/forge/messages/sourced_messages_demo.c',
        include_directories: include_directories,
        link_with: [library_forge],
        dependencies: dependencies
    )

    test(
        'lib/forge/messages/sourced_messages_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_messages_sourced_messages
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )

    demo_forge_testing_string_comparison = executable(
        'demo-forge-testing-string-comparison',
        'lib/forge-testing/string_comparison_demo.c',
        include_directories: include_directories,
        link_with: [library_forge_testing],
        dependencies: dependencies
    )

    test(
        'lib/forge-testing/string_comparison_demo.c',
        program_bash,
        args: [
            test_wrapper,
            demo_forge_testing_string_comparison
        ],
        timeout: test_timeout_seconds,
        is_parallel: test_is_parallel,
    )
endif
