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

add_library(Include STATIC)

target_sources(Include
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/MemoryOperations.cpp
        ${CMAKE_CURRENT_LIST_DIR}/StringOperations.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TypeConverter.cpp
)

target_sources(Include
    PUBLIC
    FILE_SET HEADERS
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/ISingleton.h
        ${CMAKE_CURRENT_LIST_DIR}/MemoryOperations.h
        ${CMAKE_CURRENT_LIST_DIR}/StringOperations.h
        ${CMAKE_CURRENT_LIST_DIR}/Templates.h
        ${CMAKE_CURRENT_LIST_DIR}/TypeConverter.h
        ${CMAKE_CURRENT_LIST_DIR}/Types.h
)

add_subdirectory(containers)

# Link include library to all consumers
target_link_libraries(Architecture INTERFACE Include)
target_link_libraries(Kernel INTERFACE Include)
target_link_libraries(Drivers INTERFACE Include)
