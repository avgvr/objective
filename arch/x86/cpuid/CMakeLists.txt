add_library(CPUID STATIC)
target_link_libraries(CPUID PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(CPUID PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Cpuid.cpp
)

target_sources(CPUID
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_LIST_DIR}
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/Cpuid.h
)

target_link_libraries(Architecture INTERFACE CPUID)

add_library(CPUIDASM OBJECT)

target_sources(CPUIDASM PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/CpuidCheck.nasm
)

target_link_libraries(CPUIDASM PUBLIC
    ASM_NASM_COMPILER_REQUIREMENTS
)

target_sources(ArchitectureAssemblerSources PRIVATE $<TARGET_OBJECTS:CPUIDASM>)
