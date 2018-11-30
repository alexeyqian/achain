file(GLOB HEADERS "include/achain/protocol/*.hpp")

add_library(achain_protocol 
    asset.cpp 
    ${HEADERS})

target_link_libraries( achain_protocol fc )
target_include_directories( achain_protocol
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                            "${CMAKE_CURRENT_BINARY_DIR}/include" )
