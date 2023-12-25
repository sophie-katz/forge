# Copyright (c) 2023 Sophie Katz
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
project('forge-core', 'c', default_options: ['c_std=gnu18', 'werror=true'])

# Tools
# --------------------------------------------------------------------------------------

forge_program_flex = find_program('flex')

forge_generator_flex = generator(
    forge_program_flex,
    output: '@BASENAME@.yy.c',
    arguments: ['-o', '@OUTPUT@', '@INPUT@']
)

forge_generated_files_flex = forge_generator_flex.process('lib/forge/parse/forge.l')

forge_program_bison = find_program('bison')

forge_generator_bison = generator(
    forge_program_bison,
    output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
    arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@']
)

forge_generated_files_bison = forge_generator_bison.process('lib/forge/parse/forge.y')

# Dependencies
# --------------------------------------------------------------------------------------

compiler = meson.get_compiler('c')
cmake = import('cmake')

# Subprojects
forge_subproject_unity = subproject('Unity')
forge_subproject_utf8proc = cmake.subproject('utf8proc')

# Dependencies
forge_dependency_unity = forge_subproject_unity.get_variable('unity_dep')
forge_dependency_utf8proc = forge_subproject_utf8proc.dependency('utf8proc')
forge_dependency_glib2_0 = dependency('glib-2.0')
forge_dependency_m = compiler.find_library('m', required: true)

forge_dependencies = [
    forge_dependency_utf8proc,
    forge_dependency_glib2_0,
    forge_dependency_m
]

# Forge libraries
# --------------------------------------------------------------------------------------
forge_include_directories = include_directories('include')

forge_core_lib = static_library(
    'forge-core',
    'lib/forge/ast/ast.c',
    'lib/forge/ast/debug.c',
    'lib/forge/common/color.c',
    'lib/forge/common/debug.c',
    'lib/forge/common/enums.c',
    'lib/forge/common/lexical_casts.c',
    'lib/forge/common/log.c',
    'lib/forge/common/memory.c',
    'lib/forge/common/stream.c',
    'lib/forge/exec/exec.c',
    'lib/forge/parse/parse.c',
    forge_generated_files_flex,
    forge_generated_files_bison,
    include_directories: forge_include_directories,
    dependencies: forge_dependencies,
    implicit_include_directories: true
)

# Forge Executables
# --------------------------------------------------------------------------------------

executable(
    'forge-compiler',
    'src/forge-compiler/main.c',
    include_directories: forge_include_directories,
    link_with: forge_core_lib,
    dependencies: forge_dependencies
)

executable(
    'forge-interpreter',
    'src/forge-interpreter/main.c',
    include_directories: forge_include_directories,
    link_with: forge_core_lib,
    dependencies: forge_dependencies
)

# Tests
# --------------------------------------------------------------------------------------

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

# Demos
# --------------------------------------------------------------------------------------

executable(
    'demo-common-color',
    'lib/forge/common/color_demo.c',
    include_directories: forge_include_directories,
    link_with: [forge_core_lib],
    dependencies: forge_dependencies
)

executable(
    'demo-common-log',
    'lib/forge/common/log_demo.c',
    include_directories: forge_include_directories,
    link_with: [forge_core_lib],
    dependencies: forge_dependencies
)

executable(
    'demo-common-debug',
    'lib/forge/common/debug_demo.c',
    include_directories: forge_include_directories,
    link_with: [forge_core_lib],
    dependencies: forge_dependencies
)
