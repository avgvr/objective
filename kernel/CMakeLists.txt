target_link_options(CXX_LINKER_REQUIREMENTS INTERFACE
    LINKER:-T${CMAKE_CURRENT_LIST_DIR}/linker.ld
)

add_library(Kernel INTERFACE)

add_library(Main STATIC)
target_sources(Main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/kernel.cpp
)
target_link_libraries(Main PUBLIC CXX_COMPILER_REQUIREMENTS Architecture Include)
target_link_libraries(Kernel INTERFACE Main)

add_subdirectory(details)
