#include "Vehicle.hpp"
// #include <uuid_v4_old/uuid_v4_old.h>
#include <algorithm>
#include "util.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

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

    // UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    // this->uuid = uuidGenerator.getUUID().str();
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
    // Prevent driving during prohibited conditions
    if (!started || distance <= 0) {
        return false;
    }

    mileage += distance;
    return true;
}

bool Vehicle::addDriver(Person *driver) {
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

bool Vehicle::removePassenger(Person *passenger) {
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

bool Vehicle::changeColor(const std::string &newColor) {
    if (newColor.empty() || color == newColor) {
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

double Vehicle::getPrice() const {
    return price;
}

std::string Vehicle::getName() const {
    return name;
}

json Vehicle::serializeToJSON() {
    json serialized = {};

    // Store all important info into JSON object
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
    // serialized["driver"] = driver; // Are these seriously necessary? I feel like these don't really need to be saved...
    // serialized["passengers'] = passengers;
    serialized["started"] = started;
    serialized["color"] = color;
    serialized["uuid"] = uuid;

    return serialized;
}

Vehicle Vehicle::deserializeFromJSON(const json &data) {
    // Ensure that all keys are there
    std::vector<std::string> requiredKeys = {"uuid", "name", "price", "wheels", "doors", "seats",
                                             "maxPassengers", "maxSpeed", "manufacturer", "mileage", "horsepower",
                                             "started", "color"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }
    }

    // Initialize Vehicle using all the info
    return {data["name"].get<std::string>(), data["price"].get<double>(), data["wheels"].get<int>(),
            data["doors"].get<int>(), data["seats"].get<int>(), data["maxPassengers"].get<int>(),
            data["manufacturer"].get<std::string>(), data["mileage"].get<double>(), data["horsepower"].get<double>(),
            data["maxSpeed"].get<double>(), data["color"].get<std::string>()};
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

/*
Vehicle Vehicle::loadFromUUID(std::string uuid) {
    std::string fname = "data/vehicles/" + uuid + ".json";
    if (!fs::exists(fname)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(fname);
    json importedJSON;
    file >> importedJSON;

    return Vehicle::deserializeFromJSON(importedJSON);
}
 */

std::ostream & operator <<(std::ostream &out, const Vehicle &obj) {
    out << "Vehicle Name: " << obj.name << "\n";
    out << "  Price: $" << FormatWithCommas(obj.price) << "\n";
    out << "  Wheels: " << obj.wheels << "\n";
    out << "  Doors: " << obj.doors << "\n";
    out << "  Seats: " << obj.seats << "\n";
    out << "  Max passengers: " << obj.maxPassengers << "\n";
    out << "  Manufacturer: " << obj.manufacturer << "\n";
    out << "  Mileage: " << obj.mileage << " km" << "\n";
    out << "  Horsepower: " << obj.horsepower << " hp" << "\n";
    out << "  Max speed: " << obj.maxSpeed << " km/h" << "\n";
    out << "  Color: " << obj.color << "\n";

    return out;
}
