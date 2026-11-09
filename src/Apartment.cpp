#include "Apartment.h"

Apartment::Apartment()
{
}

Apartment::Apartment(const std::list<std::string>& businesses)
    : Business(businesses)
{
}
