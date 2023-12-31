// Copyright (c) 2023 Sophie Katz
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

#include <forge/common/log.h>
#include <forge/common/memory.h>
#include <forge/cli/option_set.h>

frg_status_t _frg_cli_get_short_name_index(int* index, char short_name) {
    if (index == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (short_name >= 'a' && short_name <= 'z') {
        *index = short_name - 'a';
        return FRG_STATUS_OK;
    } else if (short_name >= 'A' && short_name <= 'Z') {
        *index = short_name - 'A' + 26;
        return FRG_STATUS_OK;
    } else {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }
}

frg_status_t frg_cli_option_set_new(
    frg_cli_option_set_t** option_set
) {
    if (option_set == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*option_set != NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    frg_status_t result = frg_safe_malloc((void**)option_set, sizeof(frg_cli_option_set_t));
    if (result != FRG_STATUS_OK) {
        return result;
    }

    (*option_set)->options = NULL;
    memset((*option_set)->options_by_short_name, 0, sizeof((*option_set)->options_by_short_name));
    (*option_set)->options_by_long_name = g_hash_table_new(g_str_hash, g_str_equal);

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_set_destroy(
    frg_cli_option_set_t** option_set
) {
    if (option_set == NULL || *option_set == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    for (GList* option = (*option_set)->options; option != NULL; option = option->next) {
        frg_status_t result = frg_cli_option_destroy((frg_cli_option_t**)&option->data);
        if (result != FRG_STATUS_OK) {
            frg_log_internal_error("unable to destroy option: %s", frg_status_to_string(result)); 
            return result;
        }
    }

    g_list_free((*option_set)->options);

    g_hash_table_destroy((*option_set)->options_by_long_name);

    return frg_safe_free((void**)option_set);
}

frg_status_t frg_cli_option_set_add_option(
    frg_cli_option_set_t* option_set,
    frg_cli_option_t* option
) {
    if (option_set == NULL || option == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    option_set->options = g_list_append(option_set->options, option);

    if (option->short_name != FRG_CLI_OPTION_SHORT_NAME_NULL) {
        int index = 0;
        frg_status_t result = _frg_cli_get_short_name_index(&index, option->short_name);
        if (result != FRG_STATUS_OK) {
            frg_log_internal_error("unable to get short name index: %s", frg_status_to_string(result));
            return result;
        }

        if (option_set->options_by_short_name[index] != NULL) {
            return FRG_STATUS_ERROR_DUPLICATE;
        }

        option_set->options_by_short_name[index] = option;
    }

    if (g_hash_table_contains(option_set->options_by_long_name, option->long_name)) {
        return FRG_STATUS_ERROR_DUPLICATE;
    } else {
        g_hash_table_insert(option_set->options_by_long_name, (void*)option->long_name, option);
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_set_get_option_by_long_name(
    frg_cli_option_t** option,
    frg_cli_option_set_t* option_set,
    const char* name
) {
    if (option == NULL || option_set == NULL || name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*option != NULL || name[0] == '-') {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (*name == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    *option = (frg_cli_option_t*)g_hash_table_lookup(option_set->options_by_long_name, name);

    if (*option == NULL) {
        return FRG_STATUS_ERROR_KEY_NOT_FOUND;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_set_get_option_by_short_name(
    frg_cli_option_t** option,
    frg_cli_option_set_t* option_set,
    char name
) {
    if (option == NULL || option_set == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*option != NULL || !frg_cli_is_valid_short_name(name)) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    }

    int index = -1;
    frg_status_t result = _frg_cli_get_short_name_index(&index, name);
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to get short name index: %s", frg_status_to_string(result)); 
        return result;
    }

    if (index < 0 || index >= FRG_CLI_OPTION_SHORT_NAME_MAX_COUNT) {
        frg_log_internal_error("invalid index %i", index);
        return FRG_STATUS_ERROR_KEY_NOT_FOUND;
    }

    *option = option_set->options_by_short_name[index];

    if (*option == NULL) {
        return FRG_STATUS_ERROR_KEY_NOT_FOUND;
    }

    return FRG_STATUS_OK;
}

frg_status_t frg_cli_option_set_print_help(
    const frg_cli_option_set_t* option_set
) {
    if (option_set == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    }

    bool first = true;

    for (GList* option = option_set->options; option != NULL; option = option->next) {
        if (first) {
            first = false;
        } else {
            printf("\n");
        }

        frg_status_t result = frg_cli_option_print_help((const frg_cli_option_t*)option->data);
        if (result != FRG_STATUS_OK) {
            frg_log_internal_error("unable to print option help: %s", frg_status_to_string(result)); 
            return result;
        }
    }

    return FRG_STATUS_OK;
}

frg_status_t _frg_cli_option_set_parse_next_long(
    frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data,
    const char* name
) {
    // Error checking
    if (option_set == NULL || argi == NULL || argv == NULL || name == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*argi < 0 || argc <= 0 || *argi >= argc || argv[*argi] == NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (argv[*argi][0] == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    } else if (name[0] == 0) {
        frg_log_fatal_error("invalid argument '--'");
        return FRG_STATUS_CLI_ERROR;
    }

    frg_cli_option_t* option = NULL;
    frg_status_t result = frg_cli_option_set_get_option_by_long_name(
        &option,
        option_set,
        name
    );
    if (result == FRG_STATUS_ERROR_KEY_NOT_FOUND) {
        frg_log_fatal_error("unknown argument '--%s'", name);
        return FRG_STATUS_CLI_ERROR;
    } else if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to get option by long name: %s", frg_status_to_string(result)); 
        return result;
    }

    return frg_cli_option_parse_next(
        option,
        argi,
        argc,
        argv,
        user_data
    );
}

frg_status_t _frg_cli_option_set_parse_next_short(
    frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data,
    char name
) {
    // Error checking
    if (option_set == NULL || argi == NULL || argv == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*argi < 0 || argc <= 0 || *argi >= argc || argv[*argi] == NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (argv[*argi][0] == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    frg_cli_option_t* option = NULL;
    frg_status_t result = frg_cli_option_set_get_option_by_short_name(
        &option,
        option_set,
        name
    );
    if (result != FRG_STATUS_OK) {
        frg_log_internal_error("unable to get option by short name: %s", frg_status_to_string(result)); 
        return result;
    }

    return frg_cli_option_parse_next(
        option,
        argi,
        argc,
        argv,
        user_data
    );
}

frg_status_t frg_cli_option_set_parse_next(
    frg_cli_option_set_t* option_set,
    int* argi,
    int argc,
    const char** argv,
    void* user_data
) {
    // Error checking
    if (option_set == NULL || argi == NULL || argv == NULL) {
        return FRG_STATUS_ERROR_NULL_ARGUMENT;
    } else if (*argi >= argc) {
        return FRG_STATUS_CLI_NO_MORE_ARGUMENTS;
    } else if (*argi < 0 || argc <= 0 || argv[*argi] == NULL) {
        return FRG_STATUS_ERROR_UNEXPECTED_ARGUMENT_VALUE;
    } else if (argv[*argi][0] == 0) {
        return FRG_STATUS_ERROR_EMPTY_STRING;
    }

    // Handle positional arguments
    if (argv[*argi][0] != '-') {
        return FRG_STATUS_CLI_REACHED_POSITIONAL_ARGUMENT;
    }

    // Try to parse the next argument
    if (argv[*argi][1] == '-') {
        // It is a long argument like --argument
        return _frg_cli_option_set_parse_next_long(
            option_set,
            argi,
            argc,
            argv,
            user_data,
            argv[*argi] + 2
        );
    } else {
        // It is a short argument like -a
        return _frg_cli_option_set_parse_next_short(
            option_set,
            argi,
            argc,
            argv,
            user_data,
            argv[*argi][1]
        );
    }
}
