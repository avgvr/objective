add_library(Port STATIC)
target_link_libraries(Port PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(Port PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/PortOps.cpp
)

target_sources(Port
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/PortOps.h
            ${CMAKE_CURRENT_LIST_DIR}/Operations
)

target_link_libraries(Architecture INTERFACE Port)
