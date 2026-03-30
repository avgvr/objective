list(APPEND CMAKE_MESSAGE_CONTEXT libgcc)
message(STATUS "Searching CRT objects")

macro(setup_crt_object_file object_file target_name)
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} --print-file-name=${object_file}
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE PATH
        ERROR_VARIABLE ERROR_MESSAGE
        RESULT_VARIABLE RESULT
    )

    if(RESULT)
        message(FATAL_ERROR ${ERROR_MESSAGE})
    endif()

    message(STATUS "${object_file} was found at the ${PATH} path")
    add_library(${target_name} OBJECT IMPORTED GLOBAL)
    set_target_properties(${target_name} PROPERTIES
        IMPORTED_OBJECTS ${PATH}
    )

endmacro()

# TODO: set platform specific CRT objects
setup_crt_object_file(crtbegin.o CRTBEGIN)
setup_crt_object_file(crtend.o CRTEND)

message(STATUS "The CRT objects were found")
add_subdirectory(${ObjectiveProject_ARCHITECTURE})
