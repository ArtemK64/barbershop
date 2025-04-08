#include "Administrator.h"
#include "Database.h"
#include "Logger.h"
#include <libpq-fe.h>
#include <string>

Administrator::Administrator(int id, const std::string& first, const std::string& middle, const std::string& last,
    const std::string& email, const std::string& city, const std::string& street,
    const std::string& houseNumber, int apartment, const std::string& inn,
    const std::string& phone)
    : Employee(id, first, middle, last, email, city, street, houseNumber, apartment, inn),
    phoneNumber(phone) {
}

std::string Administrator::getPhoneNumber() const { return phoneNumber; }
void Administrator::setPhoneNumber(const std::string& val) { phoneNumber = val; }

void Administrator::create() {
    Employee::create();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO administrators (employee_id, phone_number) VALUES ($1, $2) RETURNING id;";
    std::string empIdStr = std::to_string(Employee::getId());
    const char* values[2] = { empIdStr.c_str(), phoneNumber.c_str() };
    PGresult* res = PQexecParams(conn, query, 2, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK)
        id = std::stoi(PQgetvalue(res, 0, 0));
    else
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Administrator::read() {
    Employee::read();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "SELECT phone_number FROM administrators WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
        phoneNumber = PQgetvalue(res, 0, 0);
    }
    else {
        Logger::logError("Administrator not found");
    }
    PQclear(res);
}

void Administrator::update() {
    Employee::update();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE administrators SET phone_number = $1 WHERE employee_id = $2;";

    std::string idStr = std::to_string(getId());
    const char* values[2] = { phoneNumber.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Administrator::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM administrators WHERE employee_id = $1;";

    std::string idStr = std::to_string(getId());
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
    Employee::remove();
}