#pragma once
#include "Employee.h"

class Barber : public Employee {
private:
    int yearsOfExperience;

public:
    Barber() = default;
    Barber(int id, const std::string& first, const std::string& middle, const std::string& last, const std::string& email,
        const std::string& city, const std::string& street, const std::string& houseNumber, int apartment,
        const std::string& inn, int experience);

    int getYearsOfExperience() const;
    void setYearsOfExperience(int);

    void create() override;
    void read() override;
    void update() override;
    void remove() override;
};