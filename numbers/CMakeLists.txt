cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(grpc-number-server CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(app)