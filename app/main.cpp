#include "../lib/app/include/server.hpp"
#include "../lib/storage/include/PostgresStorageManager.hpp"
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <iostream>

int main(){
    try{
        auto db = std::make_shared<PostgresStorageManager>("host=app-postgres3.0 port=5432 dbname=postgres user=user password=1065");
        if (!db->is_open()){
            std::cout << "Not connect\n";
            return -1;
        }
	Server service(db);
	grpc::ServerBuilder builder;

    //порт, указанный для докера
	builder.AddListeningPort("0.0.0.0:9999", grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	server->Wait();
    }
    catch(const std::exception& e){
        std::cout << e.what();
    }
    return 0;
}