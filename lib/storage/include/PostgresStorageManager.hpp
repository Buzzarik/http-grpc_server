#ifndef POSTGRES_STORAGE_MANAGER_H
#define POSTGRES_STORAGE_MANAGER_H
#include "IStorageManager.hpp"
#include <pqxx/pqxx>

constexpr std::string_view query_by_number = "SELECT a.id, \"number\", cost, date, receiver, address, type_pay, type_delivered, article, name, count, price FROM \"Orders\" AS a \
JOIN \"DetailsOrder\" AS b \
ON a.id = b.id_order \
WHERE \"number\" = '{}' \
ORDER BY a.id ASC";

constexpr std::string_view query_by_date_cost = "SELECT a.id, \"number\", cost, date, receiver, address, type_pay, type_delivered, article, name, count, price FROM \"Orders\" AS a \
JOIN \"DetailsOrder\" AS b \
ON a.id = b.id_order \
WHERE date = '{}' AND cost > {} \
ORDER BY a.id ASC";

constexpr std::string_view query_by_interval_name = "SELECT a.id, \"number\", cost, date, receiver, address, type_pay, type_delivered, article, name, count, price FROM \"Orders\" AS a \
JOIN \"DetailsOrder\" AS b \
ON a.id = b.id_order \
WHERE date BETWEEN '{}' AND '{}' AND a.id NOT IN (SELECT id_order \
FROM \"DetailsOrder\" WHERE name = '{}') \
ORDER BY a.id ASC";

constexpr std::string_view query_create_order = "INSERT INTO \"Orders\"(\"number\", cost, date, receiver, address, type_pay, type_delivered) \
VALUES ('{}', {}, '{}', '{}', '{}', '{}', '{}') RETURNING id";

constexpr std::string_view query_create_detail_order = "INSERT INTO \"DetailsOrder\"( \
article, name, count, price, id_order) \
VALUES ('{}', '{}', {}, {}, {})";

class PostgresStorageManager final : public IStorageManager{
    public:
    PostgresStorageManager() = delete;
    PostgresStorageManager(std::string_view str_conn);

    ResponseStorage get_by_number(std::string_view number) override;
    ResponseStorage get_by_date_and_cost(std::string_view date, int64_t cost) override;
    ResponseStorage get_by_intervals_and_not_name(std::string_view from, std::string_view to, std::string_view name) override;
    bool create_order(InfoOrders&& info_order) override;
    bool is_open() const override;

    private:
    ResponseStorage create_list(pqxx::result&& result);
    pqxx::connection conn;
};


#endif