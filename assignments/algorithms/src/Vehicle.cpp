#include <filesystem>
#include <utility>
#include "Vehicle.hpp"
#include "util.hpp"

namespace fs = std::filesystem;

Vehicle::Vehicle(std::string name, double price, int wheels, int doors, int seats,
                 double mileage, double horsepower, double maxSpeed) {
    this->name = std::move(name);
    this->price = price;
    this->wheels = wheels;
    this->doors = doors;
    this->seats = seats;
    this->mileage = mileage;
    this->horsepower = horsepower;
    this->maxSpeed = maxSpeed;
}

bool Vehicle::changePrice(double newPrice) {
    if (newPrice <= 0) {
        // Can't have a negative price
        return false;
    }

    price = newPrice;
    return true;
}

double Vehicle::getMaxSpeed() const {
    return maxSpeed;
}

int Vehicle::getWheels() const {
    return wheels;
}

int Vehicle::getDoors() const {
    return doors;
}

int Vehicle::getSeats() const {
    return seats;
}

double Vehicle::getHorsepower() const {
    return horsepower;
}

double Vehicle::getPrice() const {
    return price;
}

std::string Vehicle::getName() const {
    return name;
}

std::ostream &operator<<(std::ostream &out, const Vehicle &obj) {
    out << "Vehicle Name: " << obj.name
        << ", Price: $" << formatWithCommas(obj.price)
        << ", Mileage: " << formatWithCommas(obj.mileage) << " km"
        << ", Max speed: " << obj.maxSpeed << " km/h";

    return out;
}