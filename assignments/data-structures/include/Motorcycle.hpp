#pragma once
#include "Vehicle.hpp"

class Motorcycle : public Vehicle {
public:
    Motorcycle(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed, std::string color);
    std::string serializeToJSON() override;
};