get_property(IS_INITIALIZED GLOBAL PROPERTY CONFFILE_MODULE SET)

if(NOT IS_INITIALIZED)
    set_property(GLOBAL PROPERTY CONFFILE_MODULE TRUE)
    set(CONFFILE_NAME "CMakeFeaturesDefineEnum.txt" CACHE STRING "Build configuration
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

function(writerunfile)
    set(OUT_FILE "${CMAKE_BINARY_DIR}/${CONFFILE_NAME}")

    get_directory_property(cacheVars CACHE_VARIABLES)

    parseFeatures(cacheVars OUT_FILE)
endfunction()
