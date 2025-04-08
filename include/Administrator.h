#pragma once
#include "Employee.h"

class Administrator : public Employee {
private:
    std::string phoneNumber;

public:
    Administrator() = default;
    Administrator(int id, const std::string& first, const std::string& middle, const std::string& last, const std::string& email,
        const std::string& city, const std::string& street, const std::string& houseNumber, int apartment,
        const std::string& inn, const std::string& phoneNumber);

    std::string getPhoneNumber() const;
    void setPhoneNumber(const std::string&);

    void create() override;
    void read() override;
    void update() override;
    void remove() override;
};