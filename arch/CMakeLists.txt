add_library(Architecture INTERFACE)

add_library(Initialization STATIC)
target_link_libraries(Initialization PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(Initialization PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/SecondaryInit.cpp
)

target_sources(Initialization
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/SecondaryInit.h
)

add_subdirectory(${ARCH})
