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
