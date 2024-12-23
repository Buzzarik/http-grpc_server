#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <string>
#include <vector>

struct Orders{
    int64_t cost;
    std::string number;
    std::string date;
    std::string receiver;
    std::string address;
    std::string type_pay;
    std::string type_delivered;
};

struct DetailOrders{
    int64_t count;
    int64_t price;
    std::string article;
    std::string name;
};

struct InfoOrders{
    Orders order;
    std::vector<DetailOrders> detail_orders;
};

#endif