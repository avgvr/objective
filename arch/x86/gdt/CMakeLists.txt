add_library(GDT STATIC)
target_link_libraries(GDT PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(GDT PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Gdt.cpp
)

target_sources(GDT
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/Gdt.h
)

target_link_libraries(Architecture INTERFACE GDT)

add_library(GDTASM OBJECT)

target_sources(GDTASM PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/SwitchToGdt.nasm
)

target_sources(ArchitectureAssemblerSources PRIVATE $<TARGET_OBJECTS:GDTASM>)
