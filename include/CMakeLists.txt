add_library(Include STATIC)

target_sources(Include
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/MemoryOperations.cpp
        ${CMAKE_CURRENT_LIST_DIR}/StringOperations.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TypeConverter.cpp
)

target_sources(Include
    PUBLIC
    FILE_SET HEADERS
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/ISingleton.h
        ${CMAKE_CURRENT_LIST_DIR}/MemoryOperations.h
        ${CMAKE_CURRENT_LIST_DIR}/StringOperations.h
        ${CMAKE_CURRENT_LIST_DIR}/Templates.h
        ${CMAKE_CURRENT_LIST_DIR}/TypeConverter.h
        ${CMAKE_CURRENT_LIST_DIR}/Types.h
)

add_subdirectory(containers)

# Link include library to all consumers
target_link_libraries(Architecture INTERFACE Include)
target_link_libraries(Kernel INTERFACE Include)
target_link_libraries(Drivers INTERFACE Include)
