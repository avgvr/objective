target_link_options(CXX_LINKER_REQUIREMENTS INTERFACE
    LINKER:-L${CMAKE_CURRENT_LIST_DIR}
)

target_sources(Initialization PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/PrimaryInit.cpp
)

target_sources(Initialization
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/PrimaryInit.h
)

target_link_libraries(Architecture INTERFACE Initialization)

# Create library for assembler build
add_library(ArchitectureAssemblerSources STATIC)

add_library(Boot OBJECT)
target_sources(Boot PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/boot.nasm
)

add_subdirectory(cpuid)
add_subdirectory(idt)
add_subdirectory(gdt)
add_subdirectory(ic)
add_subdirectory(io)
add_subdirectory(msr)
add_subdirectory(port)

target_link_libraries(Architecture INTERFACE ArchitectureAssemblerSources)
