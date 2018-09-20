cmake_minimum_required(VERSION 3.8)
project(achain)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/local/Cellar/boost@1.60/1.60.0")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/Cellar/boost@1.60/1.60.0/lib")

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

set(SOURCE_FILES main.cpp src/libraries/protocol/block_header.hpp src/libraries/protocol/base.cpp src/libraries/protocol/base.hpp src/libraries/protocol/authority.hpp src/libraries/protocol/types.hpp src/programs/achaind/main.cpp src/libraries/manifest/plugins.cpp src/libraries/app/application.cpp)
add_executable(achain ${SOURCE_FILES})