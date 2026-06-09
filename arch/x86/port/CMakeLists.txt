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

add_library(Port STATIC)
target_link_libraries(Port PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(Port PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/PortOps.cpp
)

target_sources(Port
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/PortOps.h
            ${CMAKE_CURRENT_LIST_DIR}/Operations
)

target_link_libraries(Architecture INTERFACE Port)
