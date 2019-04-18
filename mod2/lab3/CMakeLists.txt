cmake_minimum_required(VERSION 3.13)
project(lab3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

add_executable(lab3 main.cpp)