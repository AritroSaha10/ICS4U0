#include "Vehicle.hpp"

Vehicle::Vehicle(std::string name, double price, int wheels, int doors, int seats, int maxPassengers,
                 std::string manufacturer, double mileage, double horsepower, std::string color) {
    this->name = name;
    this->price = price;
    this->wheels = wheels;
    this->doors = doors;
    this->seats = seats;
    this->maxPassengers = maxPassengers;
    this->manufacturer = manufacturer;
    this->mileage = mileage;
    this->horsepower = horsepower;
    this->color = color;
}

bool Vehicle::start() {
    if (started) {
        // Don't allow starting while started
        return false;
    }

    started = true;
    return true;
}

bool Vehicle::stop() {
    if (!started) {
        // Don't allow stopping while stopped
        return false;
    }

    started = false;
    return true;
}

bool Vehicle::drive(double distance) {
    // Prevent driving during prohibited conditions
    if (!started || distance <= 0) {
        return false;
    }

    mileage += distance;
    return true;
}

bool Vehicle::addDriver(Person* driver) {
    if (this->driver != nullptr) {
        // Don't overwrite the driver if there already is one
        return false;
    }

    this->driver = driver;
    return true;
}

bool Vehicle::removeDriver() {
    if (this->driver == nullptr) {
        // No driver exists, nothing to remove
        return false;
    }

    this->driver = nullptr;
    return true;
}

bool Vehicle::addPassenger(Person *passenger) {
    if (passengers.size() == maxPassengers) {
        return false;
    }

    if (std::find(passengers.begin(), passengers.end(), passenger) != passengers.end()) {
        // Don't allow the same person to join as a passenger twice
        return false;
    }

    passengers.push_back(passenger);
    return true;
}

bool Vehicle::removePassenger(Person* passenger) {
    auto passengerPos = std::find(passengers.begin(), passengers.end(), passenger);
    if (passengerPos != passengers.end()) {
        // No such passenger, cannot remove
        return false;
    }

    passengers.erase(passengerPos);
    return true;
}

bool Vehicle::removePassenger(int idx) {
    if (idx > passengers.size() - 1) {
        // Out of bounds
        return false;
    }

    passengers.erase(passengers.begin() + idx);
    return true;
}

bool Vehicle::changeColor(std::string newColor) {
    if (newColor.size() == 0 || color == newColor) {
        return false;
    }

    color = newColor;
    return true;
}

bool Vehicle::changePrice(double newPrice) {
    if (newPrice <= 0) {
        return false;
    }

    price = newPrice;
    return true;
}

double Vehicle::getMaxSpeed() const {
    return maxSpeed;
}

double Vehicle::getWheels() const {
    return wheels;
}

double Vehicle::getDoors() const {
    return doors;
}

double Vehicle::getSeats() const {
    return seats;
}

double Vehicle::getHorsepower() const {
    return horsepower;
}