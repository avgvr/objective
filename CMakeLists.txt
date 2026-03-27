# Regard that file as root of whole project
# which include farther subprojects source codes

# Expected input variables:
#   ARCH: type of architecture 

set(CXX_FLAGS
    -Wall
    -ffreestanding
    -nostdinc
    -nostdlib
    -fno-rtti
    -mgeneral-regs-only
)

#---DEV PROPERTIES--- (delete in production)
# message(STATUS ${CMAKE_ASM_NASM_COMPILER})
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_CXX_COMPILER i686-elf-g++)
set(CMAKE_CXX_COMPILER_WORKS 1)
# set(CMAKE_CUSTOM_LINKER i686-elf-ld)
# set(CMAKE_CXX_LINK_EXECUTABLE
#     "ld <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
# set(CMAKE_CXX_FLAGS "-I${CMAKE_CURRENT_LIST_DIR}/include")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -Wall -ffreestanding -nostdinc -nostdlib -fno-rtti -mgeneral-regs-only")
# message(STATUS ${CMAKE_ASM_NASM_FLAGS})
# message(STATUS ${CMAKE_ASM_NASM_OBJECT_FORMAT})
# message(STATUS ${CMAKE_CXX_LINK_EXECUTABLE})
# message(STATUS ${CMAKE_CXX_LINK_FLAGS})
#--------------------------------------------

cmake_minimum_required(VERSION 3.25)

project(ObjectiveProject LANGUAGES CXX ASM_NASM)

list(APPEND CMAKE_MODULE_PATH
    "${ObjectiveProject_SOURCE_DIR}/cmake/module"
)

include(metadata)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Objective)

# Set architecture by default if not present
if(NOT ARCH IN_LIST OBJECTIVE_SUPPORTED_PROCESSORS)
    list(GET OBJECTIVE_SUPPORTED_PROCESSORS 0 ARCH)
endif()

# Create target with compiler requirements (include directories, flags and etc)
add_library(CXX_COMPILER_REQUIREMENTS INTERFACE)

# Target with linker requirements
add_library(CXX_LINKER_REQUIREMENTS INTERFACE)
target_link_options(CXX_LINKER_REQUIREMENTS INTERFACE
    ${CXX_FLAGS}
)
target_link_libraries(CXX_LINKER_REQUIREMENTS INTERFACE
    gcc
)

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(CXX_COMPILER_REQUIREMENTS INTERFACE
        ${CXX_FLAGS}
    )
endif()

target_include_directories(CXX_COMPILER_REQUIREMENTS
    INTERFACE
        ${ObjectiveProject_SOURCE_DIR}/include
)

# Subscribe compiler requirements to Object executable
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

target_link_libraries(Objective PRIVATE CXX_LINKER_REQUIREMENTS)
