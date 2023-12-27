#include <forge/config/config.h>
#include <forge/common/memory.h>
#include <forge/common/stream.h>
#include <forge/common/consts.h>

frg_status_t frg_config_new_default(frg_config_t** config) {
    frg_status_t result = frg_safe_malloc((void**)config, sizeof(frg_config_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*config)->command = FRG_CLI_COMMAND_HELP;
    (*config)->color_mode = FRG_COLOR_MODE_AUTO;
    (*config)->debug = false;
    (*config)->source_file = NULL;

    return FRG_STATUS_OK;
}

void frg_config_destroy(frg_config_t** config) {
    if (*config == NULL) {
        return;
    }

    g_string_free((*config)->source_file, true);

    frg_safe_free((void**)config);
}

bool _frg_config_parse_cli_try_get_command(frg_cli_command_t* command, const char* arg) {
    if (command == NULL || arg == NULL) {
        return false;
    }

    if (strcmp(arg, "help") == 0) {
        *command = FRG_CLI_COMMAND_HELP;
        return true;
    } else if (strcmp(arg, "dump-ast") == 0) {
        *command = FRG_CLI_COMMAND_DUMP_AST;
        return true;
    } else {
        return false;
    }
}

bool _frg_config_parse_cli_global_opts(frg_config_t* config, int* argi, int argc, const char** argv) {
    if (config == NULL || argi == NULL || argv == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    while (*argi < argc) {
        if (_frg_config_parse_cli_try_get_command(&config->command, argv[*argi])) {
            return true;
        } else if (argv[*argi][0] == '-') {
            if (argv[*argi][1] == 0) {
                frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "invalid argument '-'");
                return false;
            } else if (argv[*argi][1] == '-') {
                if (strcmp(argv[*argi], "--color-mode") == 0) {
                    *argi++;

                    if (*argi >= argc) {
                        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "--color-mode <MODE> expects a value");
                        return false;
                    } else if (strcmp(argv[*argi], "auto") == 0) {
                        config->color_mode = FRG_COLOR_MODE_AUTO;
                    } else if (strcmp(argv[*argi], "enabled") == 0) {
                        config->color_mode = FRG_COLOR_MODE_ENABLED;
                    } else if (strcmp(argv[*argi], "disabled") == 0) {
                        config->color_mode = FRG_COLOR_MODE_DISABLED;
                    } else {
                        frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "invalid value for --color-mode '%s'");
                        frg_log(FRG_LOG_SEVERITY_NOTE, "values available:");
                        frg_log(FRG_LOG_SEVERITY_NOTE, "  auto");
                        frg_log(FRG_LOG_SEVERITY_NOTE, "  enabled");
                        frg_log(FRG_LOG_SEVERITY_NOTE, "  disabled");
                        return false;
                    }
                } else if (strcmp(argv[*argi], "--debug") == 0) {
                    config->debug = true;
                }
            } else {
                frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "unknown argument '%s'", argv[*argi]);
                return false;
            }
        }
    }

    frg_log(FRG_LOG_SEVERITY_FATAL_ERROR, "no command specified");
    frg_log(FRG_LOG_SEVERITY_NOTE, "commands available:");
    frg_log(FRG_LOG_SEVERITY_NOTE, "  dump-ast");
    frg_log(FRG_LOG_SEVERITY_NOTE, "  help");
    frg_log(FRG_LOG_SEVERITY_NOTE, "  version");
    return false;
}

void _frg_config_parse_cli_print_version(const char* binary_name) {
    fprintf(FRG_STREAM_DEFAULT, "%s %d.%d.%d-%s\n", binary_name, FRG_VERSION_MAJOR, FRG_VERSION_MINOR, FRG_VERSION_PATCH, FRG_VERSION_LABEL);
}

void _frg_config_parse_cli_print_help_global(const char* binary_name) {
    _frg_config_parse_cli_print_version(binary_name);
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Usage: %s [global options] <command>\n");
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Commands:\n");
    fprintf(FRG_STREAM_DEFAULT, "  dump-ast\n");
    fprintf(FRG_STREAM_DEFAULT, "  help\n");
    fprintf(FRG_STREAM_DEFAULT, "  version\n");
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Global options:\n");
    fprintf(FRG_STREAM_DEFAULT, "  --color-mode <MODE>\n");
    fprintf(FRG_STREAM_DEFAULT, "    Set the color mode to one of:\n");
    fprintf(FRG_STREAM_DEFAULT, "    - 'auto' which uses color if it is available\n");
    fprintf(FRG_STREAM_DEFAULT, "    - 'enabled' which forces the use of color\n");
    fprintf(FRG_STREAM_DEFAULT, "    - 'disabled' which disables color in all cases\n");
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "  --debug\n");
    fprintf(FRG_STREAM_DEFAULT, "    Enables debug logging.\n");
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Run %s help <command> for more info.\n");
}

void _frg_config_parse_cli_print_help_dump_ast(const char* binary_name) {
    _frg_config_parse_cli_print_version(binary_name);
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Usage: %s [global options] dump-ast <source file *.frg>\n");
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Parses the AST from a given source file and prints it to the console.\n");
}

void _frg_config_parse_cli_print_help_help(const char* binary_name) {
    _frg_config_parse_cli_print_version(binary_name);
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Usage: %s [global options] help\n");
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Displays global help or help for a specific command.\n");
}

void _frg_config_parse_cli_print_help_version(const char* binary_name) {
    _frg_config_parse_cli_print_version(binary_name);
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Usage: %s [global options] version [command options]\n");
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Displays the version.\n");
    fprintf(FRG_STREAM_DEFAULT, "\n");
    fprintf(FRG_STREAM_DEFAULT, "Command options:\n");
    fprintf(FRG_STREAM_DEFAULT, "  -s|--short\n");
    fprintf(FRG_STREAM_DEFAULT, "    Use the short form of the version (better for programmatic use).\n");
}

bool frg_config_parse_cli(frg_config_t* config, int argc, const char** argv) {
    int argi = 1;

    if (!_frg_config_parse_cli_global_opts(config, &argi, argc, argv)) {
        return false;
    }

    if (config->command == FRG_CLI_COMMAND_DUMP_AST) {

    } else if (config->command == FRG_CLI_COMMAND_HELP) {
        
    }
}

void frg_config_print_debug(const frg_config_t* config);
