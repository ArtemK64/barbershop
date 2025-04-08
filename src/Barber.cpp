#include "Barber.h"
#include "Database.h"
#include "Logger.h"
#include <libpq-fe.h>
#include <string>
#include <vector>

Barber::Barber(int id, const std::string& first, const std::string& middle, const std::string& last,
    const std::string& email, const std::string& city, const std::string& street,
    const std::string& houseNumber, int apartment, const std::string& inn, int experience)
    : Employee(id, first, middle, last, email, city, street, houseNumber, apartment, inn),
    yearsOfExperience(experience) {
}

int Barber::getYearsOfExperience() const { return yearsOfExperience; }
void Barber::setYearsOfExperience(int val) { yearsOfExperience = val; }

void Barber::create() {
    Employee::create();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "INSERT INTO barbers (employee_id, years_of_experience) VALUES ($1, $2) RETURNING id;";
    std::string empIdStr = std::to_string(Employee::getId());
    std::string expStr = std::to_string(yearsOfExperience);
    const char* values[2] = { empIdStr.c_str(), expStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 2, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK)
        id = std::stoi(PQgetvalue(res, 0, 0));
    else
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Barber::read() {
    Employee::read();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();
    const char* query = "SELECT years_of_experience FROM barbers WHERE id = $1;";
    std::string idStr = std::to_string(id);
    const char* values[1] = { idStr.c_str() };
    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
        yearsOfExperience = std::stoi(PQgetvalue(res, 0, 0));
    }
    else {
        Logger::logError("Barber not found");
    }
    PQclear(res);
}

void Barber::update() {
    Employee::update();
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();

    const char* query = "UPDATE barbers SET years_of_experience = $1 WHERE employee_id = $2;";
    std::string expStr = std::to_string(yearsOfExperience);
    std::string idStr = std::to_string(getId());
    const char* values[2] = { expStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 2, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
}

void Barber::remove() {
    Database& db = Database::getInstance();
    PGconn* conn = db.getConnection();

    const char* query = "DELETE FROM barbers WHERE employee_id = $1;";
    std::string idStr = std::to_string(getId());
    const char* values[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(conn, query, 1, nullptr, values, nullptr, nullptr, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        Logger::logError(PQerrorMessage(conn));
    PQclear(res);
    Employee::remove();
}