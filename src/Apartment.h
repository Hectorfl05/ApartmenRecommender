#pragma once

#include <string>
#include <list>

class Apartment
{
private:
    std::list<std::string> Business;

public:
    // Constructores
    Apartment();
    explicit Apartment(const std::list<std::string>& businesses);

    // Métodos de acceso
    const std::list<std::string>& getBusiness() const { return Business; }
};
