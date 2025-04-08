#include "AppointmentService.h"
#include "Database.h"
#include "Logger.h"
#include <libpq-fe.h>
#include <string>

AppointmentService::AppointmentService(int id, int appointmentId, int serviceId)
    : id(id), appointmentId(appointmentId), serviceId(serviceId) {
}

int AppointmentService::getId() const { return id; }
void AppointmentService::setId(int val) { id = val; }

int AppointmentService::getAppointmentId() const { return appointmentId; }
void AppointmentService::setAppointmentId(int val) { appointmentId = val; }

int AppointmentService::getServiceId() const { return serviceId; }
void AppointmentService::setServiceId(int val) { serviceId = val; }

void AppointmentService::create() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO appointments_services (appointment_id, service_id) VALUES ($1, $2) RETURNING id;";
    std::string appStr = std::to_string(appointmentId);
    std::string servStr = std::to_string(serviceId);
    const char* values[2] = { appStr.c_str(), servStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 2, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK)
        id = std::stoi(PQgetvalue(res, 0, 0));
    else
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void AppointmentService::read() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "SELECT appointment_id, service_id FROM appointments_services WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
        appointmentId = std::stoi(PQgetvalue(res, 0, 0));
        serviceId = std::stoi(PQgetvalue(res, 0, 1));
    }
    else {
        Logger::logError("AppointmentService not found");
    }
    PQclear(res);
}

void AppointmentService::update() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "UPDATE appointments_services SET appointment_id = $1, service_id = $2 WHERE id = $3;";

    std::string idStr = std::to_string(id);
    std::string appStr = std::to_string(appointmentId);
    std::string servStr = std::to_string(serviceId);
    const char* values[3] = { appStr.c_str(), servStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 3, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void AppointmentService::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "DELETE FROM appointments_services WHERE id = $1;";

    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}