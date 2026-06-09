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

target_link_options(CXX_LINKER_REQUIREMENTS INTERFACE
    LINKER:-L${CMAKE_CURRENT_LIST_DIR}
)

target_sources(Initialization PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/PrimaryInit.cpp
)

target_sources(Initialization
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/PrimaryInit.h
)

target_link_libraries(Architecture INTERFACE Initialization)

# Create library for assembler build
add_library(ArchitectureAssemblerSources STATIC)

add_library(Boot OBJECT)
target_sources(Boot PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/boot.nasm
)

add_subdirectory(cpuid)
add_subdirectory(idt)
add_subdirectory(gdt)
add_subdirectory(ic)
add_subdirectory(io)
add_subdirectory(msr)
add_subdirectory(port)

target_link_libraries(Architecture INTERFACE ArchitectureAssemblerSources)
