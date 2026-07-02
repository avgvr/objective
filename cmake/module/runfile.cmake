get_property(IS_INITIALIZED GLOBAL PROPERTY CONFFILE_MODULE SET)

if(NOT IS_INITIALIZED)
    set_property(GLOBAL PROPERTY CONFFILE_MODULE TRUE)
    set(CONFFILE_NAME "CMakeReplication.txt" CACHE STRING "Build configuration
    file contains cmake run command to reproduce last run")
endif()

macro(convertCacheVarIntoArg var cachelist outfile)
    if("${var}" IN_LIST ${cachelist} AND NOT "${${var}}" STREQUAL "")
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

macro(parseToolset vars outfile)
    if("CMAKE_TOOLCHAIN_FILE" IN_LIST ${vars} AND NOT ${CMAKE_TOOLCHAIN_FILE})
        cmake_path(GET CMAKE_TOOLCHAIN_FILE FILENAME TOOLCHAIN_FILE_NAME)
        file(APPEND ${${outfile}}
            " -DCMAKE_TOOLCHAIN_FILE="
            "${ObjectiveProject_SOURCE_DIR}/cmake/toolchain/${TOOLCHAIN_FILE_NAME}")
    else()
        # If toolchain file isn't specifies, there are three things to pass
        # in cmake: architecture, c++ compiler and build type
        convertCacheVarIntoArg(ObjectiveProject_ARCHITECTURE ${vars} ${outfile})
        cmake_path(GET CMAKE_CXX_COMPILER FILENAME COMPILER_EXECUTABLE)
        if(NOT "${COMPILER_EXECUTABLE}" STREQUAL "")
            file(APPEND ${${outfile}}
                " -DCMAKE_CXX_COMPILER=${COMPILER_EXECUTABLE}")
        endif()
        convertCacheVarIntoArg(CMAKE_BUILD_TYPE ${vars} ${outfile})
    endif()
endmacro()

function(confwrite)
    set(OUT_FILE "${CMAKE_BINARY_DIR}/${CONFFILE_NAME}")

    get_directory_property(cacheVars CACHE_VARIABLES)

    file(WRITE ${OUT_FILE} "cmake")
    parseToolset(cacheVars OUT_FILE)
    parseFeatures(cacheVars OUT_FILE)
    file(APPEND ${OUT_FILE} " -B autoconfbuild")
endfunction()
