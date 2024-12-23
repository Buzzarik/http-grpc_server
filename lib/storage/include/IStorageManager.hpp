#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H
#include <memory>
#include <string_view>
#include <optional>
#include "Structures.hpp"

using ResponseStorage = std::pair<std::optional<std::vector<InfoOrders>>, std::optional<std::string>>;

class IStorageManager{
    public:
    virtual ~IStorageManager() = default;
    IStorageManager() = default;

    virtual ResponseStorage get_by_number(std::string_view number) = 0;
    virtual ResponseStorage get_by_date_and_cost(std::string_view date, int64_t cost) = 0;
    virtual ResponseStorage get_by_intervals_and_not_name(std::string_view from, std::string_view to, std::string_view name) = 0;
    virtual bool create_order(InfoOrders&& info_order) = 0;
    virtual bool is_open() const = 0;
};

#endif 
