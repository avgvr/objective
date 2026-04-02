add_library(CRTI OBJECT)
target_sources(CRTI PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/crti.nasm
)

target_link_libraries(CRTI
    PUBLIC
        ASM_NASM_COMPILER_REQUIREMENTS 
)

add_library(CRTN OBJECT)
target_sources(CRTN PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/crtn.nasm
)

target_link_libraries(CRTN
    PUBLIC
        ASM_NASM_COMPILER_REQUIREMENTS

)
