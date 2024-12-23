cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

#собираем библиотеку
project(lib-storage)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
    ./src/PostgresStorageManager.cpp
)

target_link_libraries(${PROJECT_NAME} 
    pqxx
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./include
)
