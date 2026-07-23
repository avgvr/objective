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

set(IS_CPUID_DEFINES TRUE)
ifFeatureEnables(CPUID IS_CPUID_DEFINES)

if(NOT IS_CPUID_DEFINES)
    add_library(CPUID INTERFACE)
else()

    target_compile_definitions(CXX_COMPILER_REQUIREMENTS INTERFACE
        FEATRUE_CPUID
    )

    add_library(CPUID STATIC)

    target_link_libraries(CPUID PUBLIC CXX_COMPILER_REQUIREMENTS)

    target_sources(CPUID PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Cpuid.cpp
    )

    target_sources(CPUID
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS
                ${CMAKE_CURRENT_LIST_DIR}
            FILES
                ${CMAKE_CURRENT_LIST_DIR}/Cpuid.h
    )

    target_link_libraries(Architecture INTERFACE CPUID)

    add_library(CPUIDASM OBJECT)

    target_sources(CPUIDASM PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/CpuidCheck.nasm
    )

    target_link_libraries(CPUIDASM PUBLIC
        ASM_NASM_COMPILER_REQUIREMENTS
    )

    target_sources(ArchitectureAssemblerSources PRIVATE $<TARGET_OBJECTS:CPUIDASM>)
endif()

startFeatureRecording(CPUID)

setFeatureField(NAME "cpuid extension")
setFeatureField(HASHCOMMIT "e3e68862bbc731802efc39cbfb1b6df7ab255204")
setFeatureField(DESCRIPTION "This feature makes available to use cpuid"
    "processor's function")
setFeatureField(TAGS "NEW;MULTIPLATFORM")
setFeatureField(AUTHORSHIP "Alexey Gavrilov <alexey.gavrilov@mail.com>")

saveFeatureRecording()
