#pragma once
#include <string>

class Service {
private:
    int id;
    std::string name;
    int price;

public:
    Service() = default;
    Service(int id, const std::string& name, int price);

    int getId() const;
    void setId(int);

    std::string getName() const;
    void setName(const std::string&);

    int getPrice() const;
    void setPrice(int);

    void create();
    void read();
    void update();
    void remove();
};