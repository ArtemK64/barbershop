#pragma once
#include "Person.h"
#include <string>

class Employee : public Person {
protected:
    std::string city;
    std::string street;
    std::string houseNumber;
    int apartmentNumber;
    std::string inn;

public:
    Employee() = default;
    Employee(int id, const std::string& first, const std::string& middle, const std::string& last, const std::string& email,
        const std::string& city, const std::string& street, const std::string& houseNumber, int apartment, const std::string& inn);

    std::string getCity() const;
    void setCity(const std::string&);

    std::string getStreet() const;
    void setStreet(const std::string&);

    std::string getHouseNumber() const;
    void setHouseNumber(const std::string&);

    int getApartmentNumber() const;
    void setApartmentNumber(int);

    std::string getINN() const;
    void setINN(const std::string&);

    void create() override;
    void read() override;
    void update() override;
    void remove() override;
};