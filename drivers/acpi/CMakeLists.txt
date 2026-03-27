add_library(ACPI STATIC)
target_link_libraries(ACPI PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(ACPI PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Acpi.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Ics.cpp
)

target_sources(ACPI
    PUBLIC
    FILE_SET HEADERS
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/Acpi.h
        ${CMAKE_CURRENT_LIST_DIR}/Ics.h
)

target_link_libraries(Drivers INTERFACE ACPI)
