#include "Client.h"
#include "Database.h"
#include "Logger.h"
#include <libpq-fe.h>
#include <iostream>
#include <vector>

Client::Client(int id, const std::string& first, const std::string& middle, const std::string& last,
               const std::string& email, int visits)
    : Person(id, first, middle, last, email), numberOfVisits(visits) {}

int Client::getNumberOfVisits() const { return numberOfVisits; }
void Client::setNumberOfVisits(int visits) { numberOfVisits = visits; }

void Client::create() {
    Person::create();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO clients (person_id, number_of_visits) VALUES ($1, $2) RETURNING id;";
    std::string personIdStr = std::to_string(id);
    std::string visitsStr = std::to_string(numberOfVisits);
    const char* values[2] = { personIdStr.c_str(), visitsStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 2, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK)
        id = std::stoi(PQgetvalue(res, 0, 0));
    else
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Client::read() {
    Person::read();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "SELECT number_of_visits FROM clients WHERE person_id = $1;";

    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
        numberOfVisits = std::stoi(PQgetvalue(res, 0, 0));
    } else {
        Logger::logError("Client not found");
    }
    PQclear(res);
}

void Client::update() {
    Person::update();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE clients SET number_of_visits = $1 WHERE person_id = $2;";

    std::string visitsStr = std::to_string(numberOfVisits);
    std::string personIdStr = std::to_string(id);
    const char* values[2] = { visitsStr.c_str(), personIdStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
    }
    PQclear(res);
}

void Client::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();

    const char* query = "DELETE FROM clients WHERE person_id = $1;";
    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        Logger::logError(PQerrorMessage(conn));
    }
    PQclear(res);
    Person::remove();
}