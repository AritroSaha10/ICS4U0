#include <algorithm>
#include <filesystem>
#include <fstream>
#include "vehicles/Vehicle.hpp"
#include "util.hpp"

namespace fs = std::filesystem;

Vehicle::Vehicle(std::string name, double price, int wheels, int doors, int seats, int maxPassengers,
                 std::string manufacturer, double mileage, double horsepower, double maxSpeed, std::string color, std::string type) {
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
    this->type = type;

    this->uuid = generate_uuid_v4();
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
    if (!started || distance <= 0) {
        // Don't drive if it's not started, or you're trying to drive a <=0 distance
        return false;
    }

    mileage += distance;
    return true;
}

bool Vehicle::addDriver(Person *driver) {
    if (this->driver != nullptr || driver == nullptr) {
        // Don't overwrite the driver if one exists, or replace with no driver (removeDriver should be used)
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

bool Vehicle::changeColor(const std::string &newColor) {
    if (newColor.empty() || color == newColor) {
        // Don't change if it's empty or the same
        return false;
    }

    color = newColor;
    return true;
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

json Vehicle::serializeToJSON() {
    json serialized = {};

    // Store all important info into JSON object
    // We don't include the driver pointer since it should never be saved while a person is driving
    serialized["uuid"] = uuid;
    serialized["name"] = name;
    serialized["price"] = price;
    serialized["wheels"] = wheels;
    serialized["doors"] = doors;
    serialized["seats"] = seats;
    serialized["maxPassengers"] = maxPassengers;
    serialized["maxSpeed"] = maxSpeed;
    serialized["manufacturer"] = manufacturer;
    serialized["mileage"] = mileage;
    serialized["horsepower"] = horsepower;
    serialized["started"] = false; // Make sure the car can be started in the future in case if it somehow saves during driving
    serialized["color"] = color;
    serialized["type"] = type;
    serialized["uuid"] = uuid;

    return serialized;
}

json Vehicle::getSanitizedJSON(const json &data) {
    // Ensure that all keys are there
    std::vector<std::string> requiredKeys = {"uuid", "name", "price", "wheels", "doors", "seats",
                                             "maxPassengers", "maxSpeed", "manufacturer", "mileage", "horsepower",
                                             "started", "color", "type"};
    json sanitizedData;

    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }

        sanitizedData[key] = data[key];
    }

    // Return sanitized version of data
    return sanitizedData;
}

void Vehicle::saveAsFile() {
    json serializedJSON = serializeToJSON();

    // Make data directory if needed
    if (!fs::is_directory("data") || !fs::exists("data")) { // Check if folder exists
        fs::create_directory("data");
    }

    // Make bank accounts directory if needed
    if (!fs::is_directory("data/vehicles") || !fs::exists("data/vehicles")) {
        fs::create_directory("data/vehicles");
    }

    // Write data to file
    std::ofstream file("data/vehicles/" + uuid + ".json");
    file << std::setw(4) << serializedJSON << std::endl;
    file.close();
}

std::string Vehicle::to_formatted_string() const {
    std::string out;

    out += "Vehicle Name: " + this->name + "\n";
    out += "  Price: $" + formatWithCommas(this->price) + "\n";
    out += "  Started?: ";
    out += this->started ? "Yes\n" : "No\n";
    out += "  Wheels: " + std::to_string(this->wheels) + "\n";
    out += "  Doors: " + std::to_string(this->doors) + "\n";
    out += "  Seats: " + std::to_string(this->seats) + "\n";
    out += "  Max passengers: " + std::to_string(this->maxPassengers) + "\n";
    out += "  Manufacturer: " + this->manufacturer + "\n";
    out += "  Mileage: " + std::to_string(this->mileage) + " km" + "\n";
    out += "  Horsepower: " + std::to_string(this->horsepower) + " hp" + "\n";
    out += "  Max speed: " + std::to_string(this->maxSpeed) + " km/h" + "\n";
    out += "  Color: " + this->color + "\n";

    return out;
}

std::ostream &operator<<(std::ostream &out, const Vehicle &obj) {
    out << obj.to_formatted_string();
    return out;
}
