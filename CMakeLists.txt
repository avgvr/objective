# Regard that file as root of whole project
# which include farther subprojects source codes

# Expected input variables:
#   ObjectiveProject_ARCHITECTURE: type of architecture

cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MESSAGE_CONTEXT ObjectiveProject)
project(ObjectiveProject LANGUAGES CXX ASM_NASM)

# Check on supported compiler
message(CHECK_START "Checking compiler support")
list(APPEND CMAKE_MESSAGE_INDENT "   ")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(CHECK_PASS "Found")
    message(STATUS "${CMAKE_CXX_COMPILER_ID} found compiler")
else()
    message(CHECK_PASS "Not Found")
    message(STATUS "${CMAKE_CXX_COMPILER_ID} compiler isn't supported")
endif()
list(POP_BACK CMAKE_MESSAGE_INDENT)

set(ObjectiveProject_CXX_FLAGS
    $<$<CONFIG:Debug>:-Wall -Wextra -ggdb -g3 -O0>
    $<$<CONFIG:Release>:-Wall -Werror>
    -ffreestanding
    -nostdinc
    -nostdlib
    -fno-rtti
    -mgeneral-regs-only
)

set(ObjectiveProject_ASM_NASM_FLAGS
    $<$<CONFIG:Debug>:-g -O0 -Wall>
    $<$<CONFIG:RELEASE>:-Wall>
)

list(APPEND CMAKE_MODULE_PATH
    "${ObjectiveProject_SOURCE_DIR}/cmake/module"
)

include(metadata)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Objective)

# Set architecture by default if not present
message(CHECK_START "Checking architecture support")
list(APPEND CMAKE_MESSAGE_INDENT "   ")
if(NOT ObjectiveProject_ARCHITECTURE IN_LIST OBJECTIVE_SUPPORTED_PROCESSORS)
    if(ObjectiveProject_ARCHITECTURE STREQUAL "" OR NOT DEFINED ObjectiveProject_ARCHITECTURE)
        message(CHECK_FAIL "not set")
        message(FATAL_ERROR "Architecture isn't set")
    else()
        message(CHECK_FAIL "unknown")
        message(FATAL_ERROR "${ObjectiveProject_ARCHITECTURE} - unknown architecture")
    endif()
else()
    message(CHECK_FAIL "ok")
    message(STATUS "${ObjectiveProject_ARCHITECTURE} - chosen architecture")
endif()
list(POP_BACK CMAKE_MESSAGE_INDENT)

# Create targets with compilers requirements (include directories, flags and etc)
add_library(CXX_COMPILER_REQUIREMENTS INTERFACE)
add_library(ASM_NASM_COMPILER_REQUIREMENTS INTERFACE)

# Target with linker requirements
add_library(CXX_LINKER_REQUIREMENTS INTERFACE)
target_link_options(CXX_LINKER_REQUIREMENTS INTERFACE
    ${ObjectiveProject_CXX_FLAGS}
)
target_link_libraries(CXX_LINKER_REQUIREMENTS INTERFACE
    gcc
)

# Set cxx compiler flags
target_compile_options(CXX_COMPILER_REQUIREMENTS INTERFACE
    ${ObjectiveProject_CXX_FLAGS}
)

# Set nasm compiler flags
target_compile_options(ASM_NASM_COMPILER_REQUIREMENTS INTERFACE
    ${ObjectiveProject_ASM_NASM_FLAGS}
)

target_include_directories(CXX_COMPILER_REQUIREMENTS
    INTERFACE
        ${ObjectiveProject_SOURCE_DIR}/include
)

# Subscribe compiler requirements to Objective executable
target_link_libraries(Objective PUBLIC CXX_COMPILER_REQUIREMENTS)

add_subdirectory(arch)
add_subdirectory(kernel)
add_subdirectory(drivers)
add_subdirectory(include)
add_subdirectory(libgcc)

target_sources(Objective PRIVATE
    $<TARGET_OBJECTS:Boot>
)

target_link_libraries(Objective PRIVATE
    CRTI
    Architecture
    Kernel
    Drivers
    CRTN
)

# Set cxx linker options
target_link_libraries(Objective PRIVATE CXX_LINKER_REQUIREMENTS)

# Targets to show output of generator expression
add_custom_target(options
    COMMENT "Toolset passed options"
    COMMAND ${CMAKE_COMMAND} -E echo
    "CXX compiler options: "
    "$<JOIN:$<TARGET_PROPERTY:CXX_COMPILER_REQUIREMENTS,INTERFACE_COMPILE_OPTIONS>, >"
    COMMAND ${CMAKE_COMMAND} -E echo
    "NASM compiler options: "
    "$<JOIN:$<TARGET_PROPERTY:ASM_NASM_COMPILER_REQUIREMENTS,INTERFACE_COMPILE_OPTIONS>, >"
    COMMAND ${CMAKE_COMMAND} -E echo
    "Linker options: "
    "$<JOIN:$<TARGET_PROPERTY:CXX_LINKER_REQUIREMENTS,INTERFACE_LINK_OPTIONS>, >"
    VERBATIM
)
