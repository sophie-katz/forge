// Copyright (c) 2024 Sophie Katz
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

#pragma once

#include <forge/common/enums.h>
#include <forge/common/types.h>
#include <stdbool.h>

void frg_log_set_minimum_severity(frg_log_severity_t severity);

typedef struct {
    bool emitted;
} frg_log_result_t;

frg_log_result_t frg_log_trace(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* format,
    ...
);

frg_log_result_t _frg_log_debug(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* format,
    ...
);

#define frg_log_debug(format, ...) _frg_log_debug( \
    __FILE__, \
    __LINE__, \
    (format), \
    ##__VA_ARGS__ \
)

frg_log_result_t _frg_log_debug_in_source_file(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* source_path,
    const char* format,
    ...
);

#define frg_log_debug_in_source_file(source_path, format, ...) \
    _frg_log_debug_in_source_file( \
        __FILE__, \
        __LINE__, \
        (source_path), \
        (format), \
        ##__VA_ARGS__ \
    )

frg_log_result_t _frg_log_debug_on_source_line(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* source_path,
    frg_lineno_t source_lineno,
    const char* format,
    ...
);

#define frg_log_debug_on_source_line(source_path, format, source_lineno, ...) \
    _frg_log_debug_on_source_line( \
        __FILE__, \
        __LINE__, \
        (source_path), \
        (format), \
        (source_lineno), \
        ##__VA_ARGS__ \
    )

frg_log_result_t _frg_log_debug_at_source_char(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    const char* format,
    ...
);

#define frg_log_debug_at_source_char(source_path, format, source_lineno, source_columnno, ...) \
    _frg_log_debug_at_source_char( \
        __FILE__, \
        __LINE__, \
        (source_path), \
        (format), \
        (source_lineno), \
        (columnno), \
        ##__VA_ARGS__ \
    )

void frg_log_note(
    const frg_log_result_t* parent_result,
    const char* format,
    ...
);

void frg_log_note_in_source_file(
    const frg_log_result_t* parent_result,
    const char* source_path,
    const char* format,
    ...
);

void frg_log_note_on_source_line(
    const frg_log_result_t* parent_result,
    const char* source_path,
    frg_lineno_t source_lineno,
    const char* format,
    ...
);

void frg_log_note_at_source_char(
    const frg_log_result_t* parent_result,
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    const char* format,
    ...
);

frg_log_result_t frg_log_warning(
    const char* format,
    ...
);

frg_log_result_t frg_log_warning_in_source_file(
    const char* source_path,
    const char* format,
    ...
);

frg_log_result_t frg_log_warning_on_source_line(
    const char* source_path,
    frg_lineno_t source_lineno,
    const char* format,
    ...
);

frg_log_result_t frg_log_warning_at_source_char(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    const char* format,
    ...
);

frg_log_result_t frg_log_error(
    const char* format,
    ...
);

frg_log_result_t frg_log_error_in_source_file(
    const char* source_path,
    const char* format,
    ...
);

frg_log_result_t frg_log_error_on_source_line(
    const char* source_path,
    frg_lineno_t source_lineno,
    const char* format,
    ...
);

frg_log_result_t frg_log_error_at_source_char(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    const char* format,
    ...
);

frg_log_result_t frg_log_fatal_error(
    const char* format,
    ...
);

frg_log_result_t _frg_log_internal_error(
    const char* log_path,
    frg_lineno_t log_lineno,
    const char* format,
    ...
);

frg_log_result_t frg_log_error_requirement_not_implemented(
    frg_requirement_id_t requirement_id,
    const char* title
);

frg_log_result_t frg_log_error_in_source_file_requirement_not_implemented(
    const char* source_path,
    frg_requirement_id_t requirement_id,
    const char* title
);

frg_log_result_t frg_log_error_on_source_line_requirement_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_requirement_id_t requirement_id,
    const char* title
);

frg_log_result_t frg_log_error_at_source_char_requirement_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_requirement_id_t requirement_id,
    const char* title
);

frg_log_result_t frg_log_error_requirement_subitem_not_implemented(
    frg_requirement_id_t requirement_id,
    frg_subitem_id_t subitem_id,
    const char* title
);

frg_log_result_t frg_log_error_in_source_file_requirement_subitem_not_implemented(
    const char* source_path,
    frg_requirement_id_t requirement_id,
    frg_subitem_id_t subitem_id,
    const char* title
);

frg_log_result_t frg_log_error_on_source_line_requirement_subitem_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_requirement_id_t requirement_id,
    frg_subitem_id_t subitem_id,
    const char* title
);

frg_log_result_t frg_log_error_at_source_char_requirement_subitem_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_requirement_id_t requirement_id,
    frg_subitem_id_t subitem_id,
    const char* title
);

frg_log_result_t frg_log_error_proposal_not_implemented(
    frg_proposal_id_t proposal_id,
    const char* title
);

frg_log_result_t frg_log_error_in_source_file_proposal_not_implemented(
    const char* source_path,
    frg_proposal_id_t proposal_id,
    const char* title
);

frg_log_result_t frg_log_error_on_source_line_proposal_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_proposal_id_t proposal_id,
    const char* title
);

frg_log_result_t frg_log_error_at_source_char_proposal_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_proposal_id_t proposal_id,
    const char* title
);

frg_log_result_t frg_log_error_proposal_subitem_not_implemented(
    frg_proposal_id_t proposal_id,
    frg_subitem_id_t subitem_id,
    const char* title
);

frg_log_result_t frg_log_error_in_source_file_proposal_subitem_not_implemented(
    const char* source_path,
    frg_proposal_id_t proposal_id,
    frg_subitem_id_t subitem_id,
    const char* title
);

frg_log_result_t frg_log_error_on_source_line_proposal_subitem_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_proposal_id_t proposal_id,
    frg_subitem_id_t subitem_id,
    const char* title
);

frg_log_result_t frg_log_error_at_source_char_proposal_subitem_not_implemented(
    const char* source_path,
    frg_lineno_t source_lineno,
    frg_columnno_t source_columnno,
    frg_proposal_id_t proposal_id,
    frg_subitem_id_t subitem_id,
    const char* title
);

#define frg_log_internal_error(format, ...) \
    _frg_log_internal_error(__FILE__, __LINE__, (format), ##__VA_ARGS__)

bool frg_log_summary_if_errors(void);
