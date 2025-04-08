#pragma once
#include <string>

class Appointment {
private:
    int id;
    int clientId;
    int barberId;
    std::string time;

public:
    Appointment() = default;
    Appointment(int id, int clientId, int barberId, const std::string& time);

    int getId() const;
    void setId(int);

    int getClientId() const;
    void setClientId(int);

    int getBarberId() const;
    void setBarberId(int);

    std::string getTime() const;
    void setTime(const std::string&);

    void create();
    void read();
    void update();
    void remove();
};