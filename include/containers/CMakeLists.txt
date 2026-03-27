add_library(ContainersHeaders INTERFACE)

target_sources(ContainersHeaders
    PUBLIC
    FILE_SET HEADERS
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/Tuple.h
)

target_link_libraries(Include PUBLIC ContainersHeaders)
