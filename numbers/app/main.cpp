#include "../lib/app/include/server.hpp"
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <iostream>

int main(){
    try{
	Server service;
	grpc::ServerBuilder builder;

    //порт, указанный для докера

	builder.AddListeningPort("0.0.0.0:8081", grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	server->Wait();
    }
    catch(const std::exception& e){
        std::cout << e.what();
    }
    return 0;
}