#include "../include/PostgresStorageManager.hpp"
#include <format>
#include <iostream>

PostgresStorageManager::PostgresStorageManager(std::string_view str_conn)
    :conn(std::string(str_conn).c_str()) {}

ResponseStorage PostgresStorageManager::create_list(pqxx::result&& result){
    if (!is_open()){
        return ResponseStorage({{}, "no connect db"});
    }
    std::vector<InfoOrders> info_orders;
    int64_t id = -1;
    for (const auto& row : result){
        DetailOrders product{
            .count = row["count"].as<int64_t>(),
            .price = row["price"].as<int64_t>(),
            .article = row["article"].as<std::string>(),
            .name = row["name"].as<std::string>()
        };
        if (id != -1 && id == row["id"].as<int64_t>()){
            info_orders.back().detail_orders.push_back(product);
        }
        else{
            id = row["id"].as<int64_t>();
            Orders order{
                .cost = row["cost"].as<int64_t>(),
                .number = row["number"].as<std::string>(),
                .date = row["date"].as<std::string>(),
                .receiver = row["receiver"].as<std::string>(),
                .address = row["address"].as<std::string>(),
                .type_pay = row["type_pay"].as<std::string>(),
                .type_delivered = row["type_delivered"].as<std::string>()
            };
            info_orders.push_back({order, {product}});
        }
    }
    ResponseStorage response;
    response.first = info_orders;
    return response;
}


ResponseStorage PostgresStorageManager::get_by_number(std::string_view number){
    if (!is_open()){
        return ResponseStorage({{}, "no connect db"});
    }
    ResponseStorage response;
    pqxx::work t(conn);
    try{
        auto result = t.exec(std::format(query_by_number, number));
        response = create_list(std::move(result));
        t.commit();
    }
    catch(const std::exception& e){
        t.abort();
        response.first.reset();
        response.second = e.what();
    }
    return response;
}

ResponseStorage PostgresStorageManager::get_by_date_and_cost(std::string_view date, int64_t cost){
//TODO
    if (!is_open()){
        return ResponseStorage({{}, "no connect db"});
    }
    ResponseStorage response;
    pqxx::work t(conn);
    try{
        auto result = t.exec(std::format(query_by_date_cost, date, cost));
        response = create_list(std::move(result));
        t.commit();
    }
    catch(const std::exception& e){
        t.abort();
        response.first.reset();
        response.second = e.what();
    }
    return response;
}

ResponseStorage PostgresStorageManager::get_by_intervals_and_not_name(std::string_view from, std::string_view to, std::string_view name){
    if (!is_open()){
        return ResponseStorage({{}, "no connect db"});
    }
    ResponseStorage response;
    pqxx::work t(conn);
    try{
        auto result = t.exec(std::format(query_by_interval_name, from, to, name));
        response = create_list(std::move(result));
        t.commit();
    }
    catch(const std::exception& e){
        t.abort();
        response.first.reset();
        response.second = e.what();
    }
    return response;
}

bool PostgresStorageManager::create_order(InfoOrders&& info_order){
    const auto& products = info_order.detail_orders;
    const auto& order = info_order.order;
    int64_t cost = 0;
    for (const auto& product : products){
        //проверяем на price > 0 and count > 0
        if (product.count <= 0 || product.price <= 0){
            return false;
        }
        cost += product.price * product.count;
    }
    pqxx::work t(conn);
    try{
        auto result = t.exec(std::format(query_create_order, 
                            order.number, //он на сервере делается
                            cost,
                            order.date,
                            order.receiver,
                            order.address,
                            order.type_pay,
                            order.type_delivered));
        int64_t id = result[0]["id"].as<int64_t>();

        for (const auto& product : products){

            t.exec(std::format(query_create_detail_order, 
                            product.article,
                            product.name,
                            product.count,
                            product.price,
                            id));
        }
        t.commit();
    }
    catch(const std::exception& e){
        t.abort();
        return false;
    }
    return true;
}

bool PostgresStorageManager::is_open() const{
    return conn.is_open();
}