add_library(MSR STATIC)
target_link_libraries(MSR PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(MSR PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Msr.cpp
)

target_sources(MSR
    PUBLIC
    FILE_SET HEADERS
        FILES
        ${CMAKE_CURRENT_LIST_DIR}/Msr.h
)

target_link_libraries(Architecture INTERFACE MSR)
