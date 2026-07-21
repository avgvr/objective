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

# Used to suppress a warning: "variable were not used by the project"
set(warnhandle ${FEATURES_SUSPEND})

#[[
 # Create a blank function when this module needs to suppress.
 # name - name of creating function.
 # args - names arguments of creating function. Arguments are list if they count
 # bigger than one. For instance, "first;second;third".
 # bodyfunc - body of the function instead of blank body if module doesn't
 # suspend.
 ]]

macro(createFeatureModuleFunction name args bodyfunc)
    if(FEATURES_SUSPEND)
        function(${name} ${args})
            message(WARNING "Feature module suspend")
        endfunction()
    else()
        function(${name} ${args})
            cmake_language(EVAL CODE "${bodyfunc}")
        endfunction()
    endif()
endmacro()

#[[
 # !IMPORTANT! Call before any other function execution and only one time.
 # !IMPORTANT! Incorrect use of this function cause undefined behaviour.
 # Filling cache variables related project information.
 ]]
createFeatureModuleFunction("featureProjectDataAssign"
    "available_archs;project_root_dir;project_arch;toolchain_dir"
[[    set(FEATURE_PROJECT_AVAILABLE_ARCHS \${\${available_archs}} CACHE STRING
        "List of available architectures by project")
    set(FEATURE_PROJECT_ROOT_DIR \${\${project_root_dir}} CACHE STRING
        "Root directory of project")
    set(FEATURE_PROJECT_ARCH \${\${projcet_arch}} CACHE STRING
        "Current project architecture")
    set(FEATURE_TOOLCHAIN_DIR \${\${toolchain_dir}} CACHE STRING
        "Toolchain directory relates project root dir")]]
)

createFeatureModuleFunction("featureModuleInitialization" ""
[[  set_property(GLOBAL PROPERTY POPULATING_FEATURES "")
    set_property(GLOBAL PROPERTY FEATURES_LIST "")
    # Features list for cannot to finish the save process
    set_property(GLOBAL PROPERTY CONSERVED_FEATURES "")

    # Name of out file
    set(FEATURE_OUT_FILE "CMakeFeaturesDefineEnum.txt" CACHE STRING "Features data
    out filename")
    set(FEATURE_AVAILABLE_FIELDS "NAME;DESCRIPTION;TAGS;HASHCOMMIT;AUTHORSHIP"
    CACHE STRING "Available feature fields")]]
)

# Run variable initialization if module is work.
featureModuleInitialization()

createFeatureModuleFunction("startFeatureRecording" "target"
[[    get_property(
        POPULATING_FEATURES_PROPERTY
        GLOBAL PROPERTY
            POPULATING_FEATURES
    )
    get_property(
        FEATURES_LIST_PROPERTY
        GLOBAL PROPERTY
            FEATURES_LIST
    )

    # Process duplicating features. Error, if it is exists.
    if(
        target IN_LIST FEATURES_LIST_PROPERTY
        OR target IN_LIST POPULATING_FEATURES_PROPERTY
    )
        message(SEND_ERROR "\\"\${target}\\" feature is duplicate")
    endif()

    if(TARGET \${target})
        list(APPEND POPULATING_FEATURES_PROPERTY \${target})
    else()
        message(SEND_ERROR "\${target} isn't a target. Feature doesn't created.")
    endif()

    # Starting the feature target record from that time
    set_property(
        GLOBAL PROPERTY
            POPULATING_FEATURES \${POPULATING_FEATURES_PROPERTY}
    )]]
)

createFeatureModuleFunction("saveFeatureRecording" ""
[[    get_property(
        POPULATING_FEATURES_PROPERTY GLOBAL PROPERTY
            POPULATING_FEATURES)
    get_property(
        FEATURES_LIST_PROPERTY GLOBAL PROPERTY
            FEATURES_LIST)
    get_property(
        CONSERVED_FEATURES_PROPERTY GLOBAL PROPERTY
            CONSERVED_FEATURES)
    list(LENGTH POPULATING_FEATURES_PROPERTY POPULATING_FEATURES_LENGTH)

    # Save feature divides on three category: nothing to save, cannot to finish
    # and can finish the save process

    if(POPULATING_FEATURES_LENGTH LESS 1)
        message(WARNING "Nothing feature to save")
    elseif(POPULATING_FEATURES_LENGTH EQUAL 1)
        list(POP_BACK POPULATING_FEATURES_PROPERTY SAVING_FEATURE)
        list(APPEND FEATURES_LIST_PROPERTY \${SAVING_FEATURE})

        # Finish the save process for all features in CONSERVED_FEATURES list
        while(NOT "\${CONSERVED_FEATURES_PROPERTY}" STREQUAL "")
            list(POP_BACK CONSERVED_FEATURES_PROPERTY CONSERVED_FEATURE)

            # Get last saved feature
            list(GET FEATURES_LIST_PROPERTY -1 LAST_SAVED_FEATURE)

            get_property(
                INHERITED_DEPENDENCIES TARGET \${LAST_FEATURE} PROPERTY
                    FEATURE_DEPENDENCIES)
            get_property(
                CONSERVED_FEATURE_DEPENDENCIES TARGET \${CONSERVED_FEATURE} PROPERTY
                    FEATURE_DEPENDENCIES)

            # Next conserved features are inherit dependencies from parent feature
            list(APPEND CONSERVED_FEATURE_DEPENDENCIES \${INHERITED_DEPENDENCIES})
            set_property(TARGET \${CONSERVED_FEATURE} PROPERTY
            FEATURE_DEPENDENCIES \${CONSERVED_FEATURE_DEPENDENCIES})

            # Save conserved feature
            list(APPEND FEATURES_LIST_PROPERTY \${CONSERVED_FEATURE})
        endwhile()
    elseif(POPULATING_FEATURES_LENGTH GREATER 1)
        list(POP_BACK POPULATING_FEATURES_PROPERTY UNFINISHED_FEATURE)
        list(APPEND CONSERVED_FEATURES_PROPERTY \${UNFINISHED_FEATURE})
    endif()

    set_property(GLOBAL PROPERTY
        POPULATING_FEATURES
            \${POPULATING_FEATURES_PROPERTY})
    set_property(GLOBAL PROPERTY
        FEATURES_LIST
            \${FEATURES_LIST_PROPERTY})
    set_property(GLOBAL PROPERTY
        CONSERVED_FEATURES
            \${CONSERVED_FEATURES_PROPERTY})]]
)

#[[
 # Available fields are: STRING:NAME, STRING:DESCRIPTION, LIST:TAGS,
 # STRING:HASHCOMMIT, STRING:AUTHORSHIP
 # NAME and HASHCOMMIT must be specified.
 ]]
createFeatureModuleFunction("setFeatureField" "field;value"
[[    get_property(
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
                TARGET \${RECORDING_FEATURE}
                PROPERTY
                    "FEATURE_\${field}" \${value}
            )
        endif()
    else()
        message(WARNING "No recording feature")
    endif()]]
)

#[[
 # Element in names must be an architecture, compiler or another feature.
 # Would is a list.
 ]]
createFeatureModuleFunction("setFeatureDependency" "names"
[[    get_property( POPULATING_FEATURES_PROPERTY GLOBAL PROPERTY POPULATING_FEATURES)
    get_property( CONSERVED_FEATURES_PROPERTY GLOBAL PROPERTY CONSERVED_FEATURES)
    list(POP_BACK POPULATING_FEATURES_PROPERTY RECORDING_FEATURE)

    get_property(FEATURES_LIST_PROPERTY GLOBAL PROPERTY FEATURES_LIST)

    if(RECORDING_FEATURE)
        get_property(availableTargetFeatures TARGET \${RECORDING_FEATURE}
            PROPERTY FEATURE_DEPENDENCIES)

        set(featdep "\${availableTargetFeatures}")

        foreach(dependency IN LISTS names)
            if(
                NOT dependency STREQUAL "\${RECORDING_FEATURE}"
                AND NOT dependency IN_LIST availableTargetFeatures
            )
                if(
                    (TARGET "\${dependency}" AND
                        (dependency IN_LIST FEATURES_LIST_PROPERTY
                        OR dependency IN_LIST CONSERVED_FEATURES_PROPERTY))
                    OR dependency IN_LIST FEATURE_AVAILABLE_ARCHS
                )
                    list(APPEND featdep \${dependency})
                # Not Target and not architecture treat as compiler,
                # since the project supported compilers list not set.
                elseif(
                    NOT TARGET "\${dependency}"
                    AND NOT dependency IN_LIST OBJECTIVE_SUPPORTED_PROCESSORЅ
                )
                    list(APPEND featdep \${dependency})
                else()
                    message(WARNING "\\"\${dependency}\\" is wrong target or architecture")
                endif()
            else()
                message(WARNING "\\"\${dependency}\\" is duplicate or refer to itself")
            endif()
        endforeach()
        set_property(TARGET \${RECORDING_FEATURE} PROPERTY FEATURE_DEPENDENCIES
        \${featdep})

    else()
        message(WARNING "No recording feature")
    endif()]]
)

#[[
 # Function that push features data into file. Content is like cmake variables.
 # <FEATURE_PROPERTY> = "<VARIABLE>"
 ]]
createFeatureModuleFunction("outFeaturesToFile" ""
[[    get_directory_property(cacheVars CACHE_VARIABLES)
    get_property(FEATURES_LIST GLOBAL PROPERTY FEATURES_LIST)

    set(OUT_FILE "\${CMAKE_BINARY_DIR}/\${FEATURE_OUT_FILE}")
    file(WRITE "\${OUT_FILE}" "# This file contains list of kernel configurable objects\n\n")

    file(APPEND "\${OUT_FILE}" "# Toolset variables\n")
    if("CMAKE_TOOLCHAIN_FILE" IN_LIST cacheVars AND NOT "\${CMAKE_TOOLCHAIN_FILE}" STREQUAL "")
        cmake_path(GET CMAKE_TOOLCHAIN_FILE FILENAME TOOLCHAIN_FILE_NAME)
        file(APPEND "\${OUT_FILE}" " -DCMAKE_TOOLCHAIN_FILE="
        "\${FEATURE_PROJECT_ROOT_DIR}/\${FEATURE_TOOLCHAIN_DIR}/\${TOOLCHAIN_FILE_NAME}")
    else()
        cmake_path(GET CMAKE_CXX_COMPILER FILENAME CXX_COMPILER_EXECUTABLE)
        file(APPEND "\${OUT_FILE}" "CMAKE_CXX_COMPILER= \\"\${CXX_COMPILER_EXECUTABLE}\\"\n")
        file(APPEND "\${OUT_FILE}" "ObjectiveProject_ARCHITECTURE = " "\\"\${ObjectiveProject_ARCHITECTURE}\\"\n")
    endif()
    file(APPEND "\${OUT_FILE}" "\n\n")
    foreach(feature IN LISTS FEATURES_LIST)
        get_property(FEATURE_DEPENDENCIES TARGET \${feature} PROPERTY FEATURE_DEPENDENCIES)

        file(APPEND "\${OUT_FILE}" "# Properties for \${feature} feature\n")

        foreach(FIELD_NAME IN LISTS FEATURE_AVAILABLE_FIELDS)
            get_property("FEATURE_\${FIELD_NAME}" TARGET \${feature} PROPERTY "FEATURE_\${FIELD_NAME}")

            file(APPEND "\${OUT_FILE}" "FEATURE_\${feature}_\${FIELD_NAME} = \\"\${FEATURE_\${FIELD_NAME}}\\"\n")
        endforeach()

        # Check if feature is enable, and push them to file
        if("FEATURE_\${feature}" IN_LIST cacheVars AND DEFINED "\${FEATURE_\${feature}}")
            file(APPEND "\${OUT_FILE}" "FEATURE_\${feature}_IS_ENABLING = " "\\"TRUE\\"\n")
        endif()

        file(APPEND "\${OUT_FILE}" "FEATURE_\${feature}_DEPENDENCIES = \\"\${FEATURE_DEPENDENCIES}\\"\n\n\n")

    endforeach()]]
)

createFeatureModuleFunction("bindFeaturesToBuild" ""
[[    get_directory_property(cacheVars CACHE_VARIABLES)

    # Get current enumeration of features
    list(FILTER cacheVars INCLUDE REGEX "FEATURE_[A-Za-z0-9\.]+\$")

    foreach(feature IN LISTS cacheVars)
        # Format to target name
        string(REGEX REPLACE "^FEATURE_" "" feature "\${feature}")
        get_property(dependencies TARGET \${feature} PROPERTY FEATURE_DEPENDENCIES)
        # Divide the whole dependencies list by three lists: target,
        # compiler and architecture dependencies
        set(dependenciesTarget "")
        foreach(elm IN LISTS dependencies)
            if(TARGET "\${elm}")
                list(APPEND dependenciesTarget "\${elm}")
            endif()
        endforeach()
        list(REMOVE_ITEM dependencies "\${dependenciesTarget}")

        # Intersection by project available architectures and dependencies lists
        set(differenceList "\${FEATURE_AVAILABLE_ARCHS}")
        list(REMOVE_ITEM differenceList \${dependencies})
        set(dependenciesArch "\${FEATURE_AVAILABLE_ARCHS}")
        list(REMOVE_ITEM dependenciesArch \${differenceList})
        set(dependenciesCompiler "\${dependencies}")
        list(REMOVE_ITEM dependenciesCompiler \${dependenciesArch})

        # If the current toolchain doesn't match target dependencies, send the error
        cmake_path(GET CMAKE_CXX_COMPILER FILENAME COMPILER_NAME)
        if(
            (NOT dependenciesArch STREQUAL ""
                AND NOT ObjectiveProject_ARCHITECTURE IN_LIST dependenciesArch)
            OR (NOT dependenciesCompiler STREQUAL ""
                AND NOT COMPILER_NAME IN_LIST dependenciesCompiler)
        )
            message(SEND_ERROR "Toolset doesn't matches with feature \${feature}\n"
                    "TOOLSET: architecture - \${ObjectiveProject_ARCHITECTURE}, "
                    "compiler - \${COMPILER_NAME}\n"
                    "FEATURE \${feature} DEPENDENCIES: "
                    "architectures - \${dependenciesArch}, "
                    "compilers - \${dependenciesCompiler}")
        endif()
    endforeach()]]
)
