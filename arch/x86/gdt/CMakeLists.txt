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

add_library(GDT STATIC)
target_link_libraries(GDT PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(GDT PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Gdt.cpp
)

target_sources(GDT
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/Gdt.h
)

target_link_libraries(Architecture INTERFACE GDT)

add_library(GDTASM OBJECT)

target_sources(GDTASM PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/SwitchToGdt.nasm
)

target_link_libraries(GDTASM PUBLIC
    ASM_NASM_COMPILER_REQUIREMENTS
)

target_sources(ArchitectureAssemblerSources PRIVATE $<TARGET_OBJECTS:GDTASM>)
