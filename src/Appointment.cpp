#include "Appointment.h"
#include "Database.h"
#include "Logger.h"
#include <libpq-fe.h>
#include <string>

Appointment::Appointment(int id, int clientId, int barberId, const std::string& time)
    : id(id), clientId(clientId), barberId(barberId), time(time) {
}

int Appointment::getId() const { return id; }
void Appointment::setId(int val) { id = val; }

int Appointment::getClientId() const { return clientId; }
void Appointment::setClientId(int val) { clientId = val; }

int Appointment::getBarberId() const { return barberId; }
void Appointment::setBarberId(int val) { barberId = val; }

std::string Appointment::getTime() const { return time; }
void Appointment::setTime(const std::string& val) { time = val; }

void Appointment::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO appointments (client_id, barber_id, time) VALUES ($1, $2, $3) RETURNING id;";
    std::string clientStr = std::to_string(clientId);
    std::string barberStr = std::to_string(barberId);
    const char* values[3] = { clientStr.c_str(), barberStr.c_str(), time.c_str() };
    PGresult* res = PQexecParams(conn, query, 3, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK)
        id = std::stoi(PQgetvalue(res, 0, 0));
    else
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Appointment::read() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "SELECT client_id, barber_id, time FROM appointments WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
        clientId = std::stoi(PQgetvalue(res, 0, 0));
        barberId = std::stoi(PQgetvalue(res, 0, 1));
        time = PQgetvalue(res, 0, 2);
    }
    else {
        Logger::logError("Appointment not found");
    }
    PQclear(res);
}

void Appointment::update() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();

    const char* query = "UPDATE appointments SET client_id = $1, barber_id = $2, time = $3 WHERE id = $4;";
    std::string idStr = std::to_string(id);
    std::string clientStr = std::to_string(clientId);
    std::string barberStr = std::to_string(barberId);
    const char* values[4] = { clientStr.c_str(), barberStr.c_str(), time.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 4, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Appointment::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();

    const char* query = "DELETE FROM appointments WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}