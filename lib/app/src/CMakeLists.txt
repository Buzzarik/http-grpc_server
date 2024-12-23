cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(lib-api)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
    ./server.cpp
    ./main.grpc.pb.cc
    ./main.pb.cc
    ./number.grpc.pb.cc
    ./number.pb.cc
)

# Public headersx
target_include_directories(${PROJECT_NAME}
    PUBLIC ../include)

# Link libraries
option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_GRPC_GRPCPP gRPC::grpc++)

target_link_libraries(${PROJECT_NAME} PUBLIC
   lib-storage

  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)