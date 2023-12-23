# Project definition
# --------------------------------------------------------------------------------------
project('forge-compiler', 'c', default_options: ['c_std=gnu18'])

# Tools
# --------------------------------------------------------------------------------------

program_flex = find_program('flex')

generator_flex = generator(
    program_flex,
    output: '@BASENAME@.yy.c',
    arguments: ['-o', '@OUTPUT@', '@INPUT@']
)

generated_files_flex = generator_flex.process('lib/forge/parse/forge.l')

program_bison = find_program('bison')

generator_bison = generator(
    program_bison,
    output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
    arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@']
)

generated_files_bison = generator_bison.process('lib/forge/parse/forge.y')

# Dependencies
# --------------------------------------------------------------------------------------

# Subprojects
forge_subproject_unity = subproject('Unity')

# Dependencies
forge_dependency_unity = forge_subproject_unity.get_variable('unity_dep')
forge_dependency_glib2_0 = dependency('glib-2.0')

# Forge libraries
# --------------------------------------------------------------------------------------
forge_include_directories = include_directories('include')

forge_compiler_lib = static_library(
    'forge-compiler',
    'lib/forge/ast/ast.c',
    'lib/forge/common/color.c',
    'lib/forge/common/debug.c',
    'lib/forge/common/enums.c',
    'lib/forge/common/log.c',
    'lib/forge/common/memory.c',
    'lib/forge/common/stream.c',
    'lib/forge/exec/exec.c',
    'lib/forge/parse/parse.c',
    generated_files_flex,
    generated_files_bison,
    include_directories: forge_include_directories,
    dependencies: [forge_dependency_glib2_0],
    implicit_include_directories: true
)

# Forge Executables
# --------------------------------------------------------------------------------------

executable(
    'forge-compiler',
    'src/forge-compiler/main.c',
    include_directories: forge_include_directories,
    link_with: forge_compiler_lib,
    dependencies: [forge_dependency_glib2_0]
)

# Tests
# --------------------------------------------------------------------------------------

test_ast_ast = executable(
    'test-ast-ast',
    'lib/forge/ast/ast_test.c',
    include_directories: forge_include_directories,
    link_with: [forge_compiler_lib],
    dependencies: [forge_dependency_glib2_0, forge_dependency_unity]
)

test('ast/ast_test.c', test_ast_ast)

test_common_stream = executable(
    'test-common-stream',
    'lib/forge/common/stream_test.c',
    include_directories: forge_include_directories,
    link_with: [forge_compiler_lib],
    dependencies: [forge_dependency_glib2_0, forge_dependency_unity]
)

test('common/stream_test.c', test_common_stream)

test_common_color = executable(
    'test-common-color',
    'lib/forge/common/color_test.c',
    include_directories: forge_include_directories,
    link_with: [forge_compiler_lib],
    dependencies: [forge_dependency_glib2_0, forge_dependency_unity]
)

test('common/color_test.c', test_common_color)

test_common_memory = executable(
    'test-common-memory',
    'lib/forge/common/memory_test.c',
    include_directories: forge_include_directories,
    link_with: [forge_compiler_lib],
    dependencies: [forge_dependency_glib2_0, forge_dependency_unity]
)

test('common/memory_test.c', test_common_memory)

# Demos
# --------------------------------------------------------------------------------------

executable(
    'demo-common-color',
    'lib/forge/common/color_demo.c',
    include_directories: forge_include_directories,
    link_with: [forge_compiler_lib],
    dependencies: [forge_dependency_glib2_0, forge_dependency_unity]
)

executable(
    'demo-common-log',
    'lib/forge/common/log_demo.c',
    include_directories: forge_include_directories,
    link_with: [forge_compiler_lib],
    dependencies: [forge_dependency_glib2_0, forge_dependency_unity]
)

executable(
    'demo-common-debug',
    'lib/forge/common/debug_demo.c',
    include_directories: forge_include_directories,
    link_with: [forge_compiler_lib],
    dependencies: [forge_dependency_glib2_0, forge_dependency_unity]
)
