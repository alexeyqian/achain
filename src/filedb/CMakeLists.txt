cmake_minimum_required(VERSION 3.8)
project(filedb)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES($ENV{BOOST_ROOT})

set(SOURCE_FILES employee_test.cpp)
add_executable(employee_test ${SOURCE_FILES})