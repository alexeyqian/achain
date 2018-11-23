cmake_minimum_required(VERSION 3.8)
project(achain)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/local/Cellar/boost@1.60/1.60.0")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/Cellar/boost@1.60/1.60.0/lib")

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
        coroutine
        unit_test_framework)

find_package(Boost 1.60 REQUIRED COMPONENTS ${BOOST_COMPONENT})

if(NOT Boost_FOUND)
    message(">>>>> Boost not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})
link_directories("/usr/local/Cellar/boost@1.60/1.60.0/lib")
message(">> Boost include dirs: ${Boost_INCLUDE_DIRS}")
message(">> Boost library dirs: ${Boost_LIBRARY_DIRS}")

add_subdirectory(libraries)
add_subdirectory(programs)
add_subdirectory(tests)