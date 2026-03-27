add_library(IC STATIC)
target_link_libraries(IC PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(IC PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Pic.cpp
)

target_sources(IC
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/Pic.h
            ${CMAKE_CURRENT_LIST_DIR}/IPic.h
)

target_link_libraries(Architecture INTERFACE IC)
