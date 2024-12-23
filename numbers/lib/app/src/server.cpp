#include "../include/server.hpp"
#include <string>
#include <chrono>
#include <optional>
#include <iomanip>
#include <sstream>
#include <iostream>


::grpc::Status Server::GetNumberAndDate(::grpc::ServerContext* context, const ::ClientReuqest* request, ::ClientResponse* response){
    try{
        auto now = std::chrono::system_clock::now();
        auto time_since_epoch = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&time_since_epoch);
        std::stringstream ss, res;
        ss << (tm->tm_year + 1900) << '-'
            << std::setw(2) << std::setfill('0') << (tm->tm_mon + 1) << '-'
            << std::setw(2) << std::setfill('0') << tm->tm_mday;
        std::string date = ss.str();

        auto now_number = std::chrono::high_resolution_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now_number.time_since_epoch());
        auto time = std::to_string(milliseconds.count());
        std::string number = time.substr(time.size() - 6, 5);
        auto* pair = response->mutable_pair();
        pair->set_number(number);
        pair->set_date(date);
    }
    catch (const std::exception& e){
        response->clear_pair();
        std::string ex = "Number service: ";
        ex += e.what();
        response->set_reason(ex);
    }
    return grpc::Status::OK;
}
