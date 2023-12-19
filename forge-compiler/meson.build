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

generated_files_flex = generator_flex.process('lib/forge/forge.l')

program_bison = find_program('bison')

generator_bison = generator(
    program_bison,
    output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
    arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@']
)

generated_files_bison = generator_bison.process('lib/forge/forge.y')

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
    'lib/forge/ast.c',
    'lib/forge/color.c',
    'lib/forge/debug.c',
    'lib/forge/log.c',
    'lib/forge/parse.c',
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

test_ast = executable(
    'test-ast',
    'lib/forge/ast_test.c',
    include_directories: forge_include_directories,
    link_with: [forge_compiler_lib],
    dependencies: [forge_dependency_glib2_0, forge_dependency_unity]
)

test('ast', test_ast)
