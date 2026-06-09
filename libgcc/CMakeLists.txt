#[[
 * Copyright (c) 2026 Alexey Gavrilov <alexey.gavrilov@mail.com>
 *
 * This file is part of ObjectiveOS.
 *
 * ObjectiveOS is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * ObjectiveOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ObjectiveOS. If not, see <https://www.gnu.org/licenses/>. 
 ]]

list(APPEND CMAKE_MESSAGE_CONTEXT libgcc)

macro(setup_crt_object_file object_file target_name)
    message(CHECK_START "Searching ${object_file} object")
    list(APPEND CMAKE_MESSAGE_INDENT "   ")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} --print-file-name=${object_file}
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE PATH
        ERROR_VARIABLE ERROR_MESSAGE
        RESULT_VARIABLE RESULT
    )

    if(RESULT)
        message(CHECK_FAIL "internal error")
        message(FATAL_ERROR ${ERROR_MESSAGE})
    endif()

    message(CHECK_PASS "found")
    add_library(${target_name} OBJECT IMPORTED GLOBAL)
    set_target_properties(${target_name} PROPERTIES
        IMPORTED_OBJECTS ${PATH}
    )
    list(POP_BACK CMAKE_MESSAGE_INDENT)
endmacro()

# TODO: set platform specific CRT objects
setup_crt_object_file(crtbegin.o CRTBEGIN)
setup_crt_object_file(crtend.o CRTEND)

add_subdirectory(${ObjectiveProject_ARCHITECTURE})
