#pragma once
#include <string>

class Person {
protected:
    int id;
    std::string firstName;
    std::string middleName;
    std::string lastName;
    std::string email;

public:
    Person() = default;
    Person(int id, const std::string& first, const std::string& middle, const std::string& last, const std::string& email);

    int getId() const;
    void setId(int newId);

    std::string getFirstName() const;
    void setFirstName(const std::string&);

    std::string getMiddleName() const;
    void setMiddleName(const std::string&);

    std::string getLastName() const;
    void setLastName(const std::string&);

    std::string getEmail() const;
    void setEmail(const std::string&);

    virtual void create();
    virtual void read();
    virtual void update();
    virtual void remove();
};