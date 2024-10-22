cmake_minimum_required(VERSION 3.22)
project(AlgorithmsLab2)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()


add_executable(AlgorithmsLab2 main.cpp)
