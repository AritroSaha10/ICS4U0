#include "Vehicle.hpp"

Vehicle::Vehicle(std::string name, double price, int wheels, int doors, int seats, int maxPassengers,
                 std::string manufacturer, double mileage, double horsepower, double maxSpeed, std::string color) {
    this->name = name;
    this->price = price;
    this->wheels = wheels;
    this->doors = doors;
    this->seats = seats;
    this->maxPassengers = maxPassengers;
    this->manufacturer = manufacturer;
    this->mileage = mileage;
    this->horsepower = horsepower;
    this->maxSpeed = maxSpeed;
    this->color = color;
    this->driver = nullptr;
    this->started = false;
}