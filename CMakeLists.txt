cmake_minimum_required(VERSION 3.3)
project(assignment1)
project(assignment1c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES main.cpp add.c)
#add_executable(assignment1 ${SOURCE_FILES})
add_executable(assignment1 main.cpp)
add_executable(assignment1c add.cpp)