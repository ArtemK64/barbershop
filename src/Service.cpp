#include "Service.h"
#include "Database.h"
#include "Logger.h"
#include <libpq-fe.h>
#include <string>

Service::Service(int id, const std::string& name, int price)
    : id(id), name(name), price(price) {
}

int Service::getId() const { return id; }
void Service::setId(int val) { id = val; }

std::string Service::getName() const { return name; }
void Service::setName(const std::string& val) { name = val; }

int Service::getPrice() const { return price; }
void Service::setPrice(int val) { price = val; }

void Service::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO services (name, price) VALUES ($1, $2) RETURNING id;";
    std::string priceStr = std::to_string(price);
    const char* values[2] = { name.c_str(), priceStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 2, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK)
        id = std::stoi(PQgetvalue(res, 0, 0));
    else
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Service::read() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "SELECT name, price FROM services WHERE id = $1;";

    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
        name = PQgetvalue(res, 0, 0);
        price = std::stoi(PQgetvalue(res, 0, 1));
    }
    else {
        Logger::logError("Service not found");
    }
    PQclear(res);
}

void Service::update() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE services SET name = $1, price = $2 WHERE id = $3;";

    std::string priceStr = std::to_string(price);
    std::string idStr = std::to_string(id);
    const char* values[3] = { name.c_str(), priceStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 3, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Service::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM services WHERE id = $1;";

    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}