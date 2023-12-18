# Project definition
# --------------------------------------------------------------------------------------
project('forge-compiler', 'c', default_options: ['c_std=gnu18'])

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
    'lib/forge/debug.c',
    'lib/forge/list.c',
    'lib/forge/log.c',
    include_directories: forge_include_directories,
    dependencies: [forge_dependency_glib2_0]
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
