#include "Motorcycle.hpp"

Motorcycle::Motorcycle(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed,
                       std::string color) : Vehicle(name, price, 2, 0, 1, 1, manufacturer, mileage, horsepower, maxSpeed, color) {

}

std::string Motorcycle::serializeToJSON() {
    throw;
}