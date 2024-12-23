cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(grpc-server CXX)
set(CMAKE_CXX_STANDARD 20)

set(SKIP_BUILD_TEST ON CACHE BOOL "Skip building tests in the subproject?") # По умолчанию тесты включаются

add_subdirectory(deps/libpqxx build-pqxx)

add_subdirectory(lib)
add_subdirectory(app)