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

set(IS_ACPI_DEFINES TRUE)
ifFeatureEnables(MSR IS_ACPI_DEFINES)

if(NOT IS_ACPI_DEFINES)
    add_library(ACPI INTERFACE)
else()
    target_compile_definitions(CXX_COMPILER_REQUIREMENTS INTERFACE
        FEATURE_ACPI
    )

    add_library(ACPI STATIC)
    target_link_libraries(ACPI PUBLIC CXX_COMPILER_REQUIREMENTS)

    target_sources(ACPI PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Acpi.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Ics.cpp
    )

    target_sources(ACPI
        PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/Acpi.h
            ${CMAKE_CURRENT_LIST_DIR}/Ics.h
    )

    target_link_libraries(Drivers INTERFACE ACPI)
endif()

startFeatureRecording(ACPI)

setFeatureField(NAME "ACPI tables")
setFeatureField(HASHCOMMIT "515a11b818e4140845f69bb26a19d270a9e42178")
setFeatureField(DESCRIPTION "ACPI tables support")
setFeatureField(TAGS "NEW")
setFeatureField(AUTHORSHIP "Alexey Gavrilov <alexey.gavrilov@mail.com>")

saveFeaturerecording()
