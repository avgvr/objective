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

get_property(IS_INITIALZIED GLOBAL PROPERTY POPULATING_FEATURES SET)

if(NOT IS_INITIALZIED)
    set_property(GLOBAL PROPERTY POPULATING_FEATURES "")
    set_property(GLOBAL PROPERTY FEATURES_LIST "")
    # Features list that cannot to finish the save process
    set_property(GLOBAL PROPERTY CONSERVED_FEATURES "")
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
    list(LENGTH POPULATING_FEATURES POPULATING_FEATURES_LENGTH)

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
    set(AVAILABLE_FIELDS "NAME;DESCRIPTION;TAGS;HASHCOMMIT;AUTHORSHIP")

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
        if(field IN_LIST AVAILABLE_FIELDS AND value)
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
