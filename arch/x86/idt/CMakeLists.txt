add_library(IDT STATIC)
target_link_libraries(IDT PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(IDT PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Idt.cpp
)

target_sources(IDT
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/Idt.h
)

target_link_libraries(Architecture INTERFACE IDT)
