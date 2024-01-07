# Copyright (c) 2024 Sophie Katz
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

if get_option('testing').auto()
    should_build_tests = get_option('debug')
else
    should_build_tests = get_option('testing').enabled()
endif

# External programs
# --------------------------------------------------------------------------------------

program_flex = find_program(
    'flex',
    required: true,
    version: ['>=2.6.4', '<2.7']
)

generator_flex = generator(
    program_flex,
    output: '@BASENAME@.yy.c',
    arguments: ['-o', '@OUTPUT@', '@INPUT@']
)

program_bison = find_program(
    'bison',
    required: true,
    version: ['>=3.8.2', '<3.9']
)

generator_bison = generator(
    program_bison,
    output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
    arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@']
)

program_llvm_config = find_program(
    'llvm-config',
    required: true,
    version: ['>=17.0.6', '<17.1']
)

# Dependencies
# --------------------------------------------------------------------------------------

c_compiler = meson.get_compiler('c')
cmake = import('cmake')

# Subprojects
if should_build_tests
    forge_subproject_unity = subproject(
        'Unity',
        required: true
    )
endif

forge_subproject_utf8proc = cmake.subproject('utf8proc')

# Dependencies
if should_build_tests
    forge_dependency_unity = forge_subproject_unity.get_variable('unity_dep')
endif

forge_dependency_utf8proc = forge_subproject_utf8proc.dependency('utf8proc')
forge_dependency_glib2_0 = dependency('glib-2.0')
forge_dependency_m = c_compiler.find_library('m', required: true)

forge_dependencies = [
    forge_dependency_utf8proc,
    forge_dependency_glib2_0,
    forge_dependency_m
]

# LLVM
forge_llvm_config_cppflags = run_command(
    [
        program_llvm_config,
        '--cppflags'
    ],
    capture: true,
    check: true,
    
)

add_project_arguments(
    forge_llvm_config_cppflags.stdout().strip().split(' '),
    language: ['cpp']
)

forge_llvm_config_ldflags = run_command(
    [
        program_llvm_config,
        '--ldflags'
    ],
    capture: true,
    check: true
)

forge_llvm_config_libs = run_command(
    [
        program_llvm_config,
        '--libs'
    ],
    capture: true,
    check: true
)

add_project_link_arguments(
    forge_llvm_config_ldflags.stdout().strip().split(' ')
        + forge_llvm_config_libs.stdout().strip().split(' '),
    language: ['c', 'cpp']
)

# Build configuration
# --------------------------------------------------------------------------------------

forge_git_commit_sha_result = run_command(
    [
        'git',
        'rev-parse',
        'HEAD'
    ],
    capture: true,
    check: true
)

forge_configuration_data = configuration_data()
forge_configuration_data.set('version_major', '0')
forge_configuration_data.set('version_minor', '1')
forge_configuration_data.set('version_patch', '0')
forge_configuration_data.set('version_label', 'alpha')
forge_configuration_data.set('git_commit_sha', forge_git_commit_sha_result.stdout().strip())
forge_configuration_data.set('compiler_id', c_compiler.get_id())
forge_configuration_data.set('compiler_version', c_compiler.version())
forge_configuration_data.set('linker_id', c_compiler.get_linker_id())
forge_configuration_data.set('host_machine_cpu', host_machine.cpu())
forge_configuration_data.set('host_machine_system', host_machine.system())

llvm_version = program_llvm_config.version().split('.')

forge_configuration_data.set('llvm_version_major', llvm_version[0])
forge_configuration_data.set('llvm_version_minor', llvm_version[1])
forge_configuration_data.set('llvm_version_patch', llvm_version[2])

if should_build_tests
    forge_configuration_data.set('should_build_tests', '1')
else
    forge_configuration_data.set('should_build_tests', '0')
endif

forge_build_config_header = configure_file(
    input: 'build_config.h.in',
    output: 'build_config.h',
    configuration: forge_configuration_data
)

message('LLVM version: ' + program_llvm_config.version())

# Generated files
# --------------------------------------------------------------------------------------

forge_generated_files_flex = generator_flex.process('lib/forge/parsing/forge.l')

forge_generated_files_bison = generator_bison.process('lib/forge/parsing/forge.y')

# Forge libraries
# --------------------------------------------------------------------------------------
forge_include_directories = include_directories('include')

forge_core_lib = static_library(
    'forge-core',
    'lib/forge/ast/ast.c',
    'lib/forge/ast/debug.c',
    'lib/forge/ast/scope.c',
    'lib/forge/ast/visitor.c',
    'lib/forge/cli/choice.c',
    'lib/forge/cli/command.c',
    'lib/forge/cli/option_set.c',
    'lib/forge/cli/option.c',
    'lib/forge/cli/program.c',
    'lib/forge/codegen/codegen.cpp',
    'lib/forge/codegen/generators.cpp',
    'lib/forge/common/color.c',
    'lib/forge/common/debug.c',
    'lib/forge/common/enums.c',
    'lib/forge/common/lexical_casts.c',
    'lib/forge/common/log.c',
    'lib/forge/common/memory.c',
    'lib/forge/common/stream.c',
    'lib/forge/config/cli_program.c',
    'lib/forge/config/commands/common.c',
    'lib/forge/config/commands/compile.c',
    'lib/forge/config/commands/dump_ast.c',
    'lib/forge/config/commands/dump_ir.c',
    'lib/forge/config/commands/help.c',
    'lib/forge/config/commands/link.c',
    'lib/forge/config/commands/version.c',
    'lib/forge/config/config.c',
    'lib/forge/parsing/parsing.c',
    'lib/forge/verification/supported_language_features.c',
    'lib/forge/verification/verifier.c',
    forge_generated_files_flex,
    forge_generated_files_bison,
    include_directories: forge_include_directories,
    dependencies: forge_dependencies,
    implicit_include_directories: true
)

# Forge Executables
# --------------------------------------------------------------------------------------

executable(
    'forge',
    'src/forge/main.c',
    include_directories: forge_include_directories,
    link_with: forge_core_lib,
    dependencies: forge_dependencies
)

# Tests
# --------------------------------------------------------------------------------------

if should_build_tests
    # Unit
    forge_test_ast_ast = executable(
        'test-ast-ast',
        'lib/forge/ast/ast_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('ast/ast_test.c', forge_test_ast_ast)

    forge_test_ast_scope = executable(
        'test-ast-scope',
        'lib/forge/ast/scope_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('ast/scope_test.c', forge_test_ast_scope)

    forge_test_ast_visitor = executable(
        'test-ast-visitor',
        'lib/forge/ast/visitor_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('ast/visitor_test.c', forge_test_ast_visitor)

    forge_test_common_stream = executable(
        'test-common-stream',
        'lib/forge/common/stream_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('common/stream_test.c', forge_test_common_stream)

    forge_test_common_color = executable(
        'test-common-color',
        'lib/forge/common/color_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('common/color_test.c', forge_test_common_color)

    forge_test_common_memory = executable(
        'test-common-memory',
        'lib/forge/common/memory_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('common/memory_test.c', forge_test_common_memory)

    forge_test_common_lexical_casts = executable(
        'test-common-lexical-casts',
        'lib/forge/common/lexical_casts_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('common/lexical_casts_test.c', forge_test_common_lexical_casts)

    forge_test_common_log = executable(
        'test-common-log',
        'lib/forge/common/log_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('common/log_test.c', forge_test_common_log)

    forge_test_cli_option = executable(
        'test-cli-option',
        'lib/forge/cli/option_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('config/cli/option_test.c', forge_test_cli_option)

    forge_test_cli_option_set = executable(
        'test-cli-option-set',
        'lib/forge/cli/option_set_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('config/cli/option_set_test.c', forge_test_cli_option_set)

    forge_test_cli_program = executable(
        'test-cli-program',
        'lib/forge/cli/program_test.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('config/cli/program_test.c', forge_test_cli_program)

    # Integration

    forge_test_parsing_basic = executable(
        'test-parsing-basic',
        'tests/parsing/basic.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies + [
            forge_dependency_unity
        ]
    )

    test('tests/parsing/basic.c', forge_test_parsing_basic)
endif

# Demos
# --------------------------------------------------------------------------------------

if should_build_tests
    forge_demo_common_color = executable(
        'demo-common-color',
        'lib/forge/common/color_demo.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies
    )

    test('common/color_demo.c', forge_demo_common_color)

    forge_demo_common_log = executable(
        'demo-common-log',
        'lib/forge/common/log_demo.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies
    )

    test('common/log_demo.c (mode=none)', forge_demo_common_log, args: ['none'])
    test('common/log_demo.c (mode=only-errors-single)', forge_demo_common_log, args: ['only-errors-single'], should_fail: true)
    test('common/log_demo.c (mode=only-errors-multiple)', forge_demo_common_log, args: ['only-errors-multiple'], should_fail: true)
    test('common/log_demo.c (mode=only-warnings-single)', forge_demo_common_log, args: ['only-warnings-single'])
    test('common/log_demo.c (mode=only-warnings-multiple)', forge_demo_common_log, args: ['only-warnings-multiple'])
    test('common/log_demo.c (mode=both)', forge_demo_common_log, args: ['both'], should_fail: true)

    forge_demo_common_debug = executable(
        'demo-common-debug',
        'lib/forge/common/debug_demo.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies
    )

    test('common/debug_demo.c', forge_demo_common_debug)

    forge_demo_cli_choice = executable(
        'demo-cli-choice',
        'lib/forge/cli/choice_demo.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies
    )

    test('cli/choice_demo.c', forge_demo_cli_choice)

    forge_demo_cli_option = executable(
        'demo-cli-option',
        'lib/forge/cli/option_demo.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies
    )

    test('cli/option_demo.c', forge_demo_cli_option)

    forge_demo_cli_option_set = executable(
        'demo-cli-option-set',
        'lib/forge/cli/option_set_demo.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies
    )

    test('cli/option_set_demo.c', forge_demo_cli_option_set)

    forge_demo_cli_command = executable(
        'demo-cli-command',
        'lib/forge/cli/command_demo.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies
    )

    test('cli/command_demo.c', forge_demo_cli_command)

    forge_demo_cli_program = executable(
        'demo-cli-program',
        'lib/forge/cli/program_demo.c',
        include_directories: forge_include_directories,
        link_with: [forge_core_lib],
        dependencies: forge_dependencies
    )

    test('cli/program_demo.c', forge_demo_cli_program)
endif
