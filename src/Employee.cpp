#include "Employee.h"
#include "Database.h"
#include "Logger.h"
#include <libpq-fe.h>
#include <iostream>
#include <vector>

Employee::Employee(int id, const std::string& first, const std::string& middle, const std::string& last,
                   const std::string& email, const std::string& city, const std::string& street,
                   const std::string& houseNumber, int apartment, const std::string& inn)
    : Person(id, first, middle, last, email),
      city(city), street(street), houseNumber(houseNumber),
      apartmentNumber(apartment), inn(inn) {}

std::string Employee::getCity() const { return city; }
void Employee::setCity(const std::string& val) { city = val; }

std::string Employee::getStreet() const { return street; }
void Employee::setStreet(const std::string& val) { street = val; }

std::string Employee::getHouseNumber() const { return houseNumber; }
void Employee::setHouseNumber(const std::string& val) { houseNumber = val; }

int Employee::getApartmentNumber() const { return apartmentNumber; }
void Employee::setApartmentNumber(int val) { apartmentNumber = val; }

std::string Employee::getINN() const { return inn; }
void Employee::setINN(const std::string& val) { inn = val; }

void Employee::create() {
    Person::create();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO employees (person_id, city, street, house_number, apartment_number, inn) VALUES ($1, $2, $3, $4, $5, $6) RETURNING id;";
    std::vector<std::string> params = {
        std::to_string(Person::getId()),
        city, street, houseNumber, std::to_string(apartmentNumber), inn
    };
    std::vector<const char*> values = {
        params[0].c_str(), params[1].c_str(), params[2].c_str(),
        params[3].c_str(), params[4].c_str(), params[5].c_str()
    };
    PGresult* res = PQexecParams(conn, query, 6, nullptr, values.data(), nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK)
        id = std::stoi(PQgetvalue(res, 0, 0));
    else
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Employee::read() {
    Person::read();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "SELECT city, street, house_number, apartment_number, inn FROM employees WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
        city = PQgetvalue(res, 0, 0);
        street = PQgetvalue(res, 0, 1);
        houseNumber = PQgetvalue(res, 0, 2);
        apartmentNumber = std::stoi(PQgetvalue(res, 0, 3));
        inn = PQgetvalue(res, 0, 4);
    }
    else {
        Logger::logError("Employee not found");
    }
    PQclear(res);
}

void Employee::update() {
    Person::update();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();

    const char* query = "UPDATE employees SET city = $1, street = $2, house_number = $3, apartment_number = $4, inn = $5 WHERE person_id = $6;";
    std::vector<std::string> params = {
        city, street, houseNumber, std::to_string(apartmentNumber), inn, std::to_string(id)
    };
    std::vector<const char*> values = {
        params[0].c_str(), params[1].c_str(), params[2].c_str(),
        params[3].c_str(), params[4].c_str(), params[5].c_str()
    };

    PGresult* res = PQexecParams(conn, query, 6, nullptr, values.data(), nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
    }
    PQclear(res);
}

void Employee::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM employees WHERE person_id = $1;";

    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
    }
    PQclear(res);
    Person::remove();
}