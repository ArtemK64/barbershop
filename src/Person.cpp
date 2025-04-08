#include "Person.h"
#include "Database.h"
#include "Logger.h"
#include <libpq-fe.h>
#include <iostream>
#include <string>
#include <vector>

Person::Person(int id, const std::string& first, const std::string& middle, const std::string& last, const std::string& email)
    : id(id), firstName(first), middleName(middle), lastName(last), email(email) {}

int Person::getId() const { return id; }
void Person::setId(int newId) { id = newId; }

std::string Person::getFirstName() const { return firstName; }
void Person::setFirstName(const std::string& val) { firstName = val; }

std::string Person::getMiddleName() const { return middleName; }
void Person::setMiddleName(const std::string& val) { middleName = val; }

std::string Person::getLastName() const { return lastName; }
void Person::setLastName(const std::string& val) { lastName = val; }

std::string Person::getEmail() const { return email; }
void Person::setEmail(const std::string& val) { email = val; }

void Person::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO people (first_name, middle_name, last_name, email) VALUES ($1, $2, $3, $4) RETURNING id;";
    std::vector<std::string> params = { firstName, middleName, lastName, email };
    std::vector<const char*> values = { params[0].c_str(), params[1].c_str(), params[2].c_str(), params[3].c_str() };
    PGresult* res = PQexecParams(conn, query, 4, nullptr, values.data(), nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK)
        id = std::stoi(PQgetvalue(res, 0, 0));
    else
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Person::read() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "SELECT first_name, middle_name, last_name, email FROM people WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
        firstName = PQgetvalue(res, 0, 0);
        middleName = PQgetvalue(res, 0, 1);
        lastName = PQgetvalue(res, 0, 2);
        email = PQgetvalue(res, 0, 3);
    }
    else {
        Logger::logError("Person not found");
    }
    PQclear(res);
}

void Person::update() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE people SET first_name = $1, middle_name = $2, last_name = $3, email = $4 WHERE id = $5;";

    std::vector<std::string> params = { firstName, middleName, lastName, email, std::to_string(id) };
    std::vector<const char*> values = { params[0].c_str(), params[1].c_str(), params[2].c_str(), params[3].c_str(), params[4].c_str() };

    PGresult* res = PQexecParams(conn, query, 5, nullptr, values.data(), nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
    }
    PQclear(res);
}

void Person::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM people WHERE id = $1;";

    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
    }
    PQclear(res);
}