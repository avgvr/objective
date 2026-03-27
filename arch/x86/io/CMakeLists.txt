add_library(IO STATIC)
target_link_libraries(IO PUBLIC CXX_COMPILER_REQUIREMENTS)

target_sources(IO PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/VGADisplay.cpp
)

target_sources(IO
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${CMAKE_CURRENT_LIST_DIR}/VGADisplay.h
            ${CMAKE_CURRENT_LIST_DIR}/VGADisplayInfo.h
)

target_link_libraries(Architecture INTERFACE IO)
