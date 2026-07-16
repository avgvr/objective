#[[
 # Copyright (c) 2026 Alexey Gavrilov <alexey.gavrilov@mail.com>
 #
 # This file is part of ObjectiveOS.
 #
 # ObjectiveOS is free software: you can redistribute it and/or modify it under
 # the terms of the GNU General Public License as published by the Free Software
 # Foundation, either version 3 of the License, or (at your option) any later
 # version.
 #
 # ObjectiveOS is distributed in the hope that it will be useful, but WITHOUT ANY
 # WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License along with
 # ObjectiveOS. If not, see <https://www.gnu.org/licenses/>.
 ]]

include_guard(GLOBAL)

set(CONFFILE_NAME "CMakeFeaturesDefineEnum.txt" CACHE STRING "Build configuration
file contains cmake run command to reproduce last run")

macro(convertCacheVarIntoArg var cachelist outfile)
    if("${var}" IN_LIST ${cachelist} AND DEFINED "${${var}}")
        file(APPEND ${${outfile}} " -D${var}=${${var}}")
    endif()
endmacro()

macro(parseFeatures vars outfile)
    # Make copy of cache variables
    set(FEATURE_CACHE_VARS ${${vars}})
    # Stay only features enumeration
    list(FILTER FEATURE_CACHE_VARS INCLUDE REGEX "FEATURE_[A-Za-z0-9\.]+$")

    foreach(FEATURE_VAR IN LISTS FEATURE_CACHE_VARS)
        convertCacheVarIntoArg("${FEATURE_VAR}" ${vars} ${outfile})
    endforeach()
endmacro()

function(writerunfile)
    set(OUT_FILE "${CMAKE_BINARY_DIR}/${CONFFILE_NAME}")

    get_directory_property(cacheVars CACHE_VARIABLES)

    parseFeatures(cacheVars OUT_FILE)
endfunction()
