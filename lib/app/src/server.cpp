#include "../include/server.hpp"
#include "number.grpc.pb.h"
#include "number.pb.h"
#include <grpcpp/grpcpp.h>
#include <string>
#include <future>
#include <chrono>
#include <optional>
#include <iomanip>
#include <sstream>
#include <iostream>


std::pair<std::optional<DateNumber>, std::optional<std::string>> longRunningTask() {
  std::string server_address("app-numbers3.0:8081");

  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());

  auto stub = ::GrpcNumberService::NewStub(channel);

  grpc::ClientContext context;

  ClientReuqest request;
  ClientResponse response;

  grpc::Status status = stub->GetNumberAndDate(&context, request, &response);

  if (status.ok()) {
    if (response.reason().empty()) {
        DateNumber date_number;
        date_number.first = response.pair().date();
        date_number.second = response.pair().number();
        return {std::make_optional(date_number), std::nullopt};
    } else {
      return {{}, response.reason()};
    }
  } else {
    return {{}, "нельзя подключиться"};
  }
  return {};
}

Server::Server(std::shared_ptr<IStorageManager> db_)
    : db(db_) {}

void Server::create_list(::ClientSelectResponse* response, const std::vector<InfoOrders>& orders_info){
        for (const auto& order_info : orders_info){
            const auto& order = order_info.order;
            const auto& details_orders = order_info.detail_orders;
            OrderInfo *ptr_order_info = response->add_order_info();
            ptr_order_info->set_address(order.address);
            ptr_order_info->set_receiver(order.receiver);
            ptr_order_info->set_type_pay(order.type_pay);
            ptr_order_info->set_type_delivered(order.type_delivered);
            ptr_order_info->set_number(order.number);
            ptr_order_info->set_date(order.date);
            ptr_order_info->set_cost(order.cost);

            for (const auto& product : details_orders){
                ClientDetailOrder* detail_order = ptr_order_info->add_detail_orders();
                detail_order->set_arcticle(product.article);
                detail_order->set_name(product.name);
                detail_order->set_count(product.count);
                detail_order->set_price(product.price);
            }
        }
}

::grpc::Status Server::GetByNumber(::grpc::ServerContext* context, const ::ClientNumberRequest* request, ::ClientSelectResponse* response){
    auto res = db->get_by_number(request->number());

    if (!res.first.has_value()){
        response->set_reason(res.second.value());
    }
    else{
        create_list(response, res.first.value());
    }
    return grpc::Status::OK;
}

::grpc::Status Server::GetByDateAndCost(::grpc::ServerContext* context, const ::ClientDateAndCostRequest* request, ::ClientSelectResponse* response){
    auto res = db->get_by_date_and_cost(request->date(), request->cost());
    if (!res.first.has_value()){
        response->set_reason(res.second.value());
    }
    else{
        create_list(response, res.first.value());
    }
    return grpc::Status::OK;
}

::grpc::Status Server::GetByIntervalsAndName(::grpc::ServerContext* context, const ::ClientIntervalsAndNameRequest* request, ::ClientSelectResponse* response){
    auto res = db->get_by_intervals_and_not_name(request->from(), request->to(), request->name());
    if (!res.first.has_value()){
        response->set_reason(res.second.value());
    }
    else{
        create_list(response, res.first.value());
    }
    return grpc::Status::OK;
}

std::pair<std::optional<DateNumber>, std::optional<std::string>> Server::create_date_number(){
    std::future<std::pair<std::optional<DateNumber>, std::optional<std::string>>> taskFuture = std::async(std::launch::async, longRunningTask);
    std::chrono::milliseconds timeout(1000);

    if (taskFuture.wait_for(timeout) == std::future_status::timeout) {
        return {std::nullopt, "Timeout server numbers"};
    } 
    else {
   
        return taskFuture.get();
    }
}

::grpc::Status Server::CreateOrder(::grpc::ServerContext* context, const ::ClientCreateRequest* request, ::ClientCreateResponse* response){
    InfoOrders info_order;
    auto& order = info_order.order;
    auto& details_order = info_order.detail_orders;
    auto res = create_date_number();
    if (!res.first.has_value()){
        response->set_reason(res.second.value());
    }
    else{
        const auto&[date, number] = res.first.value();
        order.address = request->address();
        order.date = date;
        order.number = number;
        order.receiver = request->receiver();
        order.type_delivered = request->type_delivered();
        order.type_pay = request->type_pay();
        for (const auto& product : request->detail_orders()){
            DetailOrders detail{
                .count = product.count(),
                .price = product.price(),
                .article = product.arcticle(),
                .name = product.name()
            };
            details_order.push_back(detail);
        }
        bool is_success = db->create_order(std::move(info_order));
        if (!is_success){
        response->set_reason("Not correct create order");
        }
    }
    return grpc::Status::OK;
}