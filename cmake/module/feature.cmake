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

get_property(IS_INITIALZIED GLOBAL PROPERTY POPULATING_FEATURES SET)

if(NOT IS_INITIALZIED)
    set_property(GLOBAL PROPERTY POPULATING_FEATURES "")
    set_property(GLOBAL PROPERTY FEATURES_LIST "")
    # Features list that cannot to finish the save process
    set_property(GLOBAL PROPERTY CONSERVED_FEATURES "")

    # Name of out file
    set(FEATURE_OUT_FILE "CMakeFeatureOut.txt" CACHE STRING "Features data
    out filename")
    set(FEATURE_AVAILABLE_FIELDS "NAME;DESCRIPTION;TAGS;HASHCOMMIT;AUTHORSHIP"
    CACHE STRING "Available feature fields")
endif()

function(startFeatureRecording target)
    get_property(
        POPULATING_FEATURES_PROPERTY
        GLOBAL PROPERTY
            POPULATING_FEATURES
    )

    if(TARGET ${target})
        list(APPEND POPULATING_FEATURES_PROPERTY ${target})
    else()
        message(WARNING "${target} isn't target")
    endif()

    set_property(
        GLOBAL PROPERTY
            POPULATING_FEATURES ${POPULATING_FEATURES_PROPERTY}
    )
endfunction()

function(saveRecordingFeature)
    get_property(
        POPULATING_FEATURES_PROPERTY GLOBAL PROPERTY
            POPULATING_FEATURES)
    get_property(
        FEATURES_LIST_PROPERTY GLOBAL PROPERTY
            FEATURES_LIST)
    get_property(
        CONSERVED_FEATURES_PROPERTY GLOBAL PROPERTY
            CONSERVED_FEATURES)
    list(LENGTH POPULATING_FEATURES_PROPERTY POPULATING_FEATURES_LENGTH)

    # Divided save feature on three category: nothing to save, cannot to finish
    # and can finish
    if(POPULATING_FEATURES_LENGTH LESS 1)
        message(WARNING "Nothing feature to save")
    elseif(POPULATING_FEATURES_LENGTH EQUAL 1)
        list(POP_BACK POPULATING_FEATURES_PROPERTY SAVING_FEATURE)
        list(APPEND FEATURES_LIST_PROPERTY ${SAVING_FEATURE})
        # Finish save process for all features in CONSERVED_FEATURES list
        while(NOT "${CONSERVED_FEATURES_PROPERTY}" STREQUAL "")
            list(POP_BACK CONSERVED_FEATURES_PROPERTY CONSERVED_FEATURE)

            # Get last saved feature
            list(GET FEATURES_LIST_PROPERTY -1 LAST_SAVED_FEATURE)

            get_property(
                INHERITED_DEPENDENCIES TARGET ${LAST_FEATURE} PROPERTY
                    FEATURE_DEPENDENCIES)
            get_property(
                CONSERVED_FEATURE_DEPENDENCIES TARGET ${CONSERVED_FEATURE} PROPERTY
                    FEATURE_DEPENDENCIES)

            # Next conserved features are inherit dependencies from parent feature
            list(APPEND CONSERVED_FEATURE_DEPENDENCIES ${INHERITED_DEPENDENCIES})
            set_property(TARGET ${CONSERVED_FEATURE} PROPERTY
            FEATURE_DEPENDENCIES ${CONSERVED_FEATURE_DEPENDENCIES})

            # Save conserved feature
            list(APPEND FEATURES_LIST_PROPERTY ${CONSERVED_FEATURE})
        endwhile()
    elseif(POPULATING_FEATURES_LENGTH GREATER 1)
        list(POP_BACK POPULATING_FEATURES_PROPERTY UNFINISHED_FEATURE)
        list(APPEND CONSERVED_FEATURES_PROPERTY UNFINISHED_FEATURE)
    endif()

    set_property(GLOBAL PROPERTY
        POPULATING_FEATURES
            ${POPULATING_FEATURES_PROPERTY})
    set_property(GLOBAL PROPERTY
        FEATURES_LIST
            ${FEATURES_LIST_PROPERTY})
    set_property(GLOBAL PROPERTY
        CONSERVED_FEATURES
            ${CONSERVED_FEATURES_PROPERTY})
endfunction()

#[[
 # Available fields are: STRING:NAME, STRING:DESCRIPTION, LIST:TAGS,
 # STRING:HASHCOMMIT, STRING:AUTHORSHIP
 # NAME and HASHCOMMIT must be specified.
 ]]
function(setFeatureField field value)
    get_property(
        POPULATING_FEATURES_PROPERTY
        GLOBAL PROPERTY
            POPULATING_FEATURES
    )
    list(POP_BACK POPULATING_FEATURES_PROPERTY RECORDING_FEATURE)

    # Check on current recording feature
    if(RECORDING_FEATURE)
        # If field is available and value is define, save target property with
        # associated value
        if(field IN_LIST FEATURE_AVAILABLE_FIELDS AND value)
            set_property(
                TARGET ${RECORDING_FEATURE}
                PROPERTY
                    "FEATURE_${field}" ${value}
            )
        endif()
    else()
        message(WARNING "No recording feature")
    endif()
endfunction()

#[[
 # Element in names must be an architecture, compiler or another feature. Would be a list.
 ]]
function(setFeatureDependency names)
    get_property( POPULATING_FEATURES_PROPERTY GLOBAL PROPERTY POPULATING_FEATURES)
    list(POP_BACK POPULATING_FEATURES_PROPERTY RECORDING_FEATURE)
    list(GET -1 POPULATING_FEATURES_PROPERTY FEATURE_DEP_INHERIT)

    get_property(FEATURES_LIST_PROPERTY GLOBAL PROPERTY FEATURES_LIST)

    if(RECORDING_FEATURE)
        set(featdep "")

        foreach(dependency IN LISTS names)
            if(
                (dependency IN_LIST FEATURES_LIST_PROPERTY
                OR dependency IN_LIST POPULATING_FEATURES_PROPERTY
                AND NOT dependency STREQUAL ${RECORDING_FEATURE})
                OR (dependency IN_LIST OBJECTIVE_SUPPORTED_PROCESSORS)
                OR (dependency MATCHES "gcc" OR dependency MATCHES "clang")
            )
                list(APPEND featdep ${dependency})
            else()
                message(WARNING "\"${dependency}\" is wrong")
            endif()
        endforeach()
        set_property(TARGET ${RECORDING_FEATURE} PROPERTY FEATURE_DEPENDENCIES
        ${featdep})

    else()
        message(WARNING "No recording feature")
    endif()
endfunction()

#[[
 # Function that push features data into file. Content is like cmake variables.
 # <FEATURE_PROPERTY> = "<VARIABLE>"
 ]]
function(outFeaturesToFile)
    get_directory_property(cacheVars CACHE_VARIABLES)
    get_property(FEATURES_LIST GLOBAL PROPERTY FEATURES_LIST)

    set(OUT_FILE "${CMAKE_BINARY_DIR}/${FEATURE_OUT_FILE}")
    file(WRITE "${OUT_FILE}"
        "# This file contains list of kernel configurable objects\n\n")

    file(APPEND "${OUT_FILE}" "# Toolset variables\n")
    if("CMAKE_TOOLCHAIN_FILE" IN_LIST cacheVars
            AND NOT "${CMAKE_TOOLCHAIN_FILE}" STREQUAL "")
        cmake_path(GET CMAKE_TOOLCHAIN_FILE FILENAME TOOLCHAIN_FILE_NAME)
        file(APPEND "${OUT_FILE}"
            " -DCMAKE_TOOLCHAIN_FILE="
            "${ObjectiveProject_SOURCE_DIR}/cmake/toolchain/${TOOLCHAIN_FILE_NAME}")
    else()
        cmake_path(GET CMAKE_CXX_COMPILER FILENAME CXX_COMPILER_EXECUTABLE)
        file(APPEND "${OUT_FILE}"
            "CMAKE_CXX_COMPILER = \"${CXX_COMPILER_EXECUTABLE}\"\n")
        file(APPEND "${OUT_FILE}"
            "ObjectiveProject_ARCHITECTURE = "
            "\"${ObjectiveProject_ARCHITECTURE}\"\n")
    endif()
    file(APPEND "${OUT_FILE}" "\n\n")
    foreach(feature IN LISTS FEATURES_LIST)
        get_property(FEATURE_DEPENDENCIES TARGET ${feature} PROPERTY
        FEATURE_DEPENDENCIES)

        file(APPEND "${OUT_FILE}" "# Properties for ${feature} feature\n")

        foreach(FIELD_NAME IN LISTS FEATURE_AVAILABLE_FIELDS)
            get_property("FEATURE_${FIELD_NAME}" TARGET ${feature} PROPERTY
                "FEATURE_${FIELD_NAME}")

            file(APPEND "${OUT_FILE}"
                "FEATURE_${feature}_${FIELD_NAME} = \"${FEATURE_${FIELD_NAME}}\"\n")
        endforeach()

        # Check if feature is enable, and push them to file
        if("FEATURE_${feature}" IN_LIST cacheVars
                AND NOT "${FEATURE_${feature}}" STREQUAL "")
            file(APPEND "${OUT_FILE}"
                "FEATURE_${feature}_IS_ENABLING = "
                "\"TRUE\"\n")
        endif()

        file(APPEND "${OUT_FILE}"
            "FEATURE_${feature}_DEPENDENCIES = \"${FEATURE_DEPENDENCIES}\"\n\n\n")

    endforeach()
endfunction()
