#include <build_config.h>
#include <forge/ast/ast.h>
#include <forge/ast/debug.h>
#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/common/stream.h>
#include <forge/config/cli/program.h>
#include <forge/config/config.h>
#include <forge/parse/parse.h>

frg_status_t frg_config_new_default(frg_config_t** config) {
    frg_status_t result = frg_safe_malloc((void**)config, sizeof(frg_config_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*config)->color_mode = FRG_COLOR_MODE_AUTO;
    (*config)->debug = false;
    (*config)->source_file = NULL;
    (*config)->version_short = false;

    return FRG_STATUS_OK;
}

void frg_config_destroy(frg_config_t** config) {
    if (*config == NULL) {
        return;
    }

    g_string_free((*config)->source_file, true);

    frg_safe_free((void**)config);
}

frg_status_t _frg_cli_command_callback_dump_ast(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    if (pos_args == NULL) {
        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "command dump-ast expects a source file as an argument");
        return FRG_STATUS_CLI_ERROR;
    } else if (pos_args->next != NULL) {
        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "command dump-ast expects only one source file as an argument");
        return FRG_STATUS_CLI_ERROR;
    }

    const char* path = (const char*)pos_args->data;

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "unable to open source file: %s (%s)", path, strerror(errno));
        return FRG_STATUS_CLI_ERROR;
    }

    frg_ast_t* ast = NULL;
    frg_status_t result = frg_parse_file(
        &ast,
        file,
        path
    );
    if (result != FRG_STATUS_OK) {
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to parse source file: %s", frg_status_to_string(result));
        fclose(file);
        return FRG_STATUS_CLI_ERROR;
    }

    frg_ast_print_debug(ast, 0);

    fclose(file);

    return FRG_STATUS_OK;
}

frg_status_t _frg_config_create_cli_command_dump_ast(frg_cli_command_t** command) {
    frg_status_t result = frg_cli_command_new(
        command,
        "dump-ast",
        "source file",
        "Dump the AST of the source file.",
        _frg_cli_command_callback_dump_ast
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_command_callback_help(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    if (pos_args == NULL) {
        return frg_cli_program_print_help((frg_cli_program_t*)program, NULL);
    } else {
        return frg_cli_program_print_help((frg_cli_program_t*)program, (const char*)pos_args->data);
    }
}

frg_status_t _frg_config_create_cli_command_help(frg_cli_command_t** command) {
    frg_status_t result = frg_cli_command_new(
        command,
        "help",
        NULL,
        "Display help information.",
        _frg_cli_command_callback_help
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_command_callback_version(
    int* exit_status,
    const struct frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    const frg_config_t* config = (const frg_config_t*)user_data;

    if (config->version_short) {
        return frg_cli_program_print_version_short((frg_cli_program_t*)program);
    } else {
        return frg_cli_program_print_version_long((frg_cli_program_t*)program);
    }
}

frg_status_t _frg_cli_option_callback_version_short(
    void* user_data,
    const char* value
) {
    frg_config_t* config = (frg_config_t*)user_data;
    config->version_short = true;

    return FRG_STATUS_OK;
}

frg_status_t _frg_config_create_cli_command_version(frg_cli_command_t** command) {
    frg_status_t result = frg_cli_command_new(
        command,
        "version",
        NULL,
        "Display version information.",
        _frg_cli_command_callback_version
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(
        &option,
        "short",
        "Use short format (<major>.<minor>.<patch>-<label>)",
        _frg_cli_option_callback_version_short
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI option: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_option_set_add_option(
        (*command)->option_set,
        option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI option to command: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_program_callback(
    int* exit_status,
    const frg_cli_program_t* program,
    void* user_data,
    GList* pos_args
) {
    return frg_cli_program_print_help(program, NULL);
}

frg_status_t _frg_cli_option_callback_debug(void* user_data, const char* value) {
    frg_config_t* config = (frg_config_t*)user_data;
    config->debug = true;

    return FRG_STATUS_OK;
}

frg_status_t _frg_config_create_cli_program(frg_cli_program_t** program) {
    GString* version_details = g_string_new(NULL);
    g_string_append_printf(version_details, "Git commit: %s\n", FRG_GIT_COMMIT_SHA);
    g_string_append_printf(version_details, "Compiled with: %s %s\n", FRG_COMPILER_ID, FRG_COMPILER_VERSION);
    g_string_append_printf(version_details, "Linked with: %s\n", FRG_LINKER_ID);
    g_string_append_printf(version_details, "Built for: %s (%s)", FRG_HOST_MACHINE_SYSTEM, FRG_HOST_MACHINE_CPU);

    frg_status_t result = frg_cli_program_new(
        program,
        "Forge",
        "forge",
        NULL,
        FRG_VERSION_MAJOR,
        FRG_VERSION_MINOR,
        FRG_VERSION_PATCH,
        FRG_VERSION_LABEL,
        version_details,
        _frg_cli_program_callback
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI program: %s", frg_status_to_string(result));
        return result;
    }

    frg_cli_option_t* option = NULL;
    result = frg_cli_option_new_flag(
        &option,
        "debug",
        "Enable debug logging",
        _frg_cli_option_callback_debug
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI option: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_option_set_add_option(
        (*program)->global_options,
        option
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI option to program: %s", frg_status_to_string(result));
        return result;
    }

    frg_cli_command_t* command = NULL;
    result = _frg_config_create_cli_command_dump_ast(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_add_command(
        *program,
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI command to program: %s", frg_status_to_string(result));
        return result;
    }

    command = NULL;
    result = _frg_config_create_cli_command_help(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_add_command(
        *program,
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI command to program: %s", frg_status_to_string(result));
        return result;
    }

    command = NULL;
    result = _frg_config_create_cli_command_version(&command);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI command: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_add_command(
        *program,
        command
    );
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to add CLI command to program: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_config_parse_cli(int* exit_status, frg_config_t* config, int argc, const char** argv) {
    frg_cli_program_t* program = NULL;
    frg_status_t result = _frg_config_create_cli_program(&program);
    if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to create CLI program: %s", frg_status_to_string(result));
        return result;
    }

    result = frg_cli_program_parse(
        exit_status,
        program,
        argc,
        argv,
        config
    );
    if (result == FRG_STATUS_CLI_ERROR) {
        return FRG_STATUS_CLI_ERROR;
    } else if (result != FRG_STATUS_OK) {
        frg_log_prefix_internal();
        frg_log(FRG_LOG_SEVERITY_INTERNAL_ERROR, "unable to parse args with CLI program: %s", frg_status_to_string(result));
        return result;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_config_log_debug(const frg_config_t* config) {
    if (config == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    frg_log(FRG_LOG_SEVERITY_DEBUG, "Configuration:");
    frg_log(FRG_LOG_SEVERITY_NOTE, "config.color_mode == %i", config->color_mode);
    frg_log(FRG_LOG_SEVERITY_NOTE, "config.debug == %s", config->debug ? "true" : "false");
    frg_log(FRG_LOG_SEVERITY_NOTE, "config.source_file == %s", config->source_file != NULL ? config->source_file->str : "(null)");
    frg_log(FRG_LOG_SEVERITY_NOTE, "config.version_short == %s", config->version_short ? "true" : "false");

    return FRG_STATUS_OK;
}