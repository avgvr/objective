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

set(IS_DISPLAY_DEFINES TRUE)
ifFeatureEnables(DISPLAY IS_DISPLAY_DEFINES)

if(NOT IS_DISPLAY_DEFINES)
    add_library(DISPLAY INTERFACE)
else()
    target_compile_definitions(CXX_COMPILER_REQUIREMENTS INTERFACE
        FEATURE_DISPLAY
    )

    add_library(DISPLAY STATIC)
    target_link_libraries(DISPLAY PUBLIC CXX_COMPILER_REQUIREMENTS)

    target_sources(DISPLAY PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/VGADisplay.cpp
    )

    target_sources(DISPLAY
        PUBLIC
            FILE_SET HEADERS
            FILES
                ${CMAKE_CURRENT_LIST_DIR}/VGADisplay.h
                ${CMAKE_CURRENT_LIST_DIR}/VGADisplayInfo.h
    )

    target_link_libraries(Architecture INTERFACE DISPLAY)
endif()

startFeatureRecording(DISPLAY)

setFeatureField(NAME "VGA driver for x86 processors")
setFeatureField(HASHCOMMIT "b82ccebd3a0cce28177b5fac046139e2de7737fa")
setFeatureField(DESCRIPTION "Add ability to write into framebuffer"
                    "under the x86 processorѕ")
setFeatureField(TAGS "NEW")
setFeatureField(AUTHORSHIP "Alexey Gavrilov <alexey.gavrilov@mail.com>")
setFeatureDependencies(x86)

saveFeatureRecording()
