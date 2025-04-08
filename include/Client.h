#pragma once
#include "Person.h"

class Client : public Person {
private:
    int numberOfVisits;

public:
    Client() = default;
    Client(int id, const std::string& first, const std::string& middle, const std::string& last, const std::string& email, int visits = 0);

    int getNumberOfVisits() const;
    void setNumberOfVisits(int);

    void create() override;
    void read() override;
    void update() override;
    void remove() override;
};