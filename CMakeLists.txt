cmake_minimum_required(VERSION 3.8)
project(achain)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/local/Cellar/boost@1.60/1.60.0")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/Cellar/boost@1.60/1.60.0/lib")

set(BOOST_COMPONENTS)
list(APPEND BOOST_COMPONENTS thread
        date_time
        system
        filesystem
        program_options
        signals
        serialization
        chrono
        assert
        static_assert
        context
        locale
        coroutine)

find_package(Boost REQUIRED COMPONENTS ${BOOST_COMPONENT})

if(Boost_FOUND)
    message("Boost found")
endif()

include_directories(${Boost_INCLUDE_DIRS})
link_directories("/usr/local/Cellar/boost@1.60/1.60.0/lib")
message("Boost include dirs: ${Boost_INCLUDE_DIRS}")
message("Boost libraries: ${Boost_LIBRARIES}")

#ADD_DEFINITIONS( "-DHAS_BOOST" )

add_subdirectory(programs)
add_subdirectory(tests)

#set(SOURCE_FILES main.cpp src/libraries/protocol/block_header.hpp src/libraries/protocol/base.cpp src/libraries/protocol/base.hpp src/libraries/protocol/authority.hpp src/libraries/protocol/types.hpp src/programs/achaind/main.cpp src/libraries/manifest/plugins.cpp src/libraries/app/application.cpp)
#add_executable(achain ${SOURCE_FILES})