#include "vehicles/Vehicle.hpp"
#include "vehicles/Motorcycle.hpp"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

Motorcycle::Motorcycle(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed, std::string color, double engineSize, double maxAcceleration, MOTORCYCLE_TYPE motorcycleType) : Vehicle(std::move(name), price, 2, 0, 1, 1, std::move(manufacturer), mileage, horsepower, maxSpeed, std::move(color), "motorcycle") {
    this->engineSize = engineSize;
    this->maxAcceleration = maxAcceleration;
    this->motorcycleType = motorcycleType;
}

double Motorcycle::getEngineSize() {
    return engineSize;
}

double Motorcycle::getMaxAccel() {
    return maxAcceleration;
}

double Motorcycle::approximateFuelUsageFromKm(double kilometres) {
    // Average fuel efficiency depends on the type of motorcycle, we can predict that a stronger engine is
    // less efficient. It will also need more fuel to accelerate faster.
    // These are just ballpark equations.
    double fuelEfficiency = 0;
    switch (motorcycleType) {
        case SPORT: fuelEfficiency = 38.3; break;
        case CRUISER: fuelEfficiency = 25.5; break;
        case SCOOTER: fuelEfficiency = 29.76; break;
        case TOURING: fuelEfficiency = 22.9;
    }

    fuelEfficiency -= std::clamp((engineSize - 100) * 0.012, 0.0, 9.0);
    fuelEfficiency -= std::clamp(maxAcceleration * 0.5, 0.0, 3.5);

    // Find litres from fuel efficient and km driven using km/(km/L) = L
    double litresUsed = kilometres / fuelEfficiency;
    return litresUsed;
}

json Motorcycle::serializeToJSON() {
    json baseData = Vehicle::serializeToJSON();

    baseData["engineSize"] = engineSize;
    baseData["maxAcceleration"] = maxAcceleration;

    std::string motorcycleTypeStr;
    switch (motorcycleType) {
        case SPORT: motorcycleTypeStr = "sport"; break;
        case CRUISER: motorcycleTypeStr = "cruiser"; break;
        case SCOOTER: motorcycleTypeStr = "scooter"; break;
        case TOURING: motorcycleTypeStr = "touring";
    }

    baseData["motorcycleType"] = motorcycleType;

    return baseData;
}

Motorcycle Motorcycle::deserializeFromJSON(const json &data) {
    json sanitizedData = Vehicle::getSanitizedJSON(data);

    // Ensure that all of Motorcycle's keys are there
    std::vector<std::string> requiredKeys = {"engineSize", "maxAcceleration", "motorcycleType"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }

        sanitizedData[key] = data[key];
    }

    // Convert string representation of enum back to enum, unfortunately
    // strings don't have support with switch/case so if/else is required
    MOTORCYCLE_TYPE motorcycleType;
    std::string motorcycleTypeStr = data["motorcycleType"];
    if (motorcycleTypeStr == "sport") motorcycleType = SPORT;
    else if (motorcycleTypeStr == "cruiser") motorcycleType = CRUISER;
    else if (motorcycleTypeStr == "scooter") motorcycleType = SCOOTER;
    else if (motorcycleTypeStr == "touring") motorcycleType = TOURING;
    else throw;

    return {
            data["name"], data["price"], data["manufacturer"], data["mileage"], data["horsepower"], data["maxSpeed"],
            data["color"], data["engineSize"], data["maxAcceleration"], motorcycleType
    };
}

Motorcycle Motorcycle::loadFromUUID(const std::string& uuid) {
    std::string fname = "data/motorcycle/" + uuid + ".json";
    if (!fs::exists(fname)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(fname);
    json importedJSON;
    file >> importedJSON;

    return Motorcycle::deserializeFromJSON(importedJSON);
}

Motorcycle Motorcycle::loadFromPath(const std::string& path) {
    if (!fs::exists(path)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(path);
    json importedJSON;
    file >> importedJSON;

    return Motorcycle::deserializeFromJSON(importedJSON);
}

std::string Motorcycle::to_formatted_string() const {
    std::string baseString = Vehicle::to_formatted_string();

    std::string motorcycleTypeStr;
    switch (motorcycleType) {
        case SPORT: motorcycleTypeStr = "sport"; break;
        case CRUISER: motorcycleTypeStr = "cruiser"; break;
        case SCOOTER: motorcycleTypeStr = "scooter"; break;
        case TOURING: motorcycleTypeStr = "touring";
    }

    baseString += "  Engine size: " + std::to_string(this->engineSize) + " CC\n";
    baseString += "  Max acceleration: " + std::to_string(this->maxAcceleration) + " m/s^2\n";
    baseString += "  Motorcycle type: " + std::to_string(this->maxAcceleration) + " m/s^2\n";

    return baseString;
}