#ifndef SERVER_H
#define SERVER_H
#include "../src/main.grpc.pb.h"
#include <string_view>
#include <memory>
#include "../../storage/include/PostgresStorageManager.hpp"

using DateNumber = std::pair<std::string, std::string>;

class Server : public GrpcService::Service{
    public:
    Server(std::shared_ptr<IStorageManager> db_);
    Server() = delete;
    ::grpc::Status GetByNumber(::grpc::ServerContext* context, const ::ClientNumberRequest* request, ::ClientSelectResponse* response);
    ::grpc::Status GetByDateAndCost(::grpc::ServerContext* context, const ::ClientDateAndCostRequest* request, ::ClientSelectResponse* response);
    ::grpc::Status GetByIntervalsAndName(::grpc::ServerContext* context, const ::ClientIntervalsAndNameRequest* request, ::ClientSelectResponse* response);
    ::grpc::Status CreateOrder(::grpc::ServerContext* context, const ::ClientCreateRequest* request, ::ClientCreateResponse* response);
    void create_list(::ClientSelectResponse* response, const std::vector<InfoOrders>& order_info);
    std::pair<std::optional<DateNumber>, std::optional<std::string>> create_date_number();
    
    private:
    std::shared_ptr<IStorageManager> db;
};

#endif