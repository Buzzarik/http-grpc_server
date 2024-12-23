#ifndef SERVER_H
#define SERVER_H
#include "../src/number.grpc.pb.h"
#include <string_view>
#include <memory>

using DateNumber = std::pair<std::string, std::string>;

class Server : public GrpcNumberService::Service{
    public:
    ::grpc::Status GetNumberAndDate(::grpc::ServerContext* context, const ::ClientReuqest* request, ::ClientResponse* response);
};

#endif