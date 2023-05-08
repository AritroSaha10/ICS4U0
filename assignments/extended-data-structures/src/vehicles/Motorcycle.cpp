#include "vehicles/Vehicle.hpp"
#include "vehicles/Motorcycle.hpp"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

MOTORCYCLE_TYPE convertMotorcycleTypeStrToEnum(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });

    if (str == "sport") return SPORT;
    else if (str == "cruiser") return CRUISER;
    else if (str == "scooter") return SCOOTER;
    else if (str == "touring") return TOURING;
    throw;
}

std::string convertMotorcycleTypeEnumToStr(MOTORCYCLE_TYPE type) {
    switch (type) {
        case SPORT: return "sport";
        case CRUISER: return "cruiser";
        case SCOOTER: return "scooter";
        case TOURING: return "touring";
        default: throw;
    }
}

Motorcycle::Motorcycle(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed, std::string color, double engineSize, double maxAcceleration, MOTORCYCLE_TYPE motorcycleType) : Vehicle(std::move(name), price, 2, 0, 1, 1, std::move(manufacturer), mileage, horsepower, maxSpeed, std::move(color), "motorcycle") {
    this->engineSize = engineSize;
    this->maxAcceleration = maxAcceleration;
    this->motorcycleType = motorcycleType;
}

double Motorcycle::getEngineSize() const {
    return engineSize;
}

double Motorcycle::getMaxAccel() const {
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
    baseData["motorcycleType"] = convertMotorcycleTypeEnumToStr(motorcycleType);

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

    Motorcycle tmp{
            data["name"], data["price"], data["manufacturer"], data["mileage"], data["horsepower"], data["maxSpeed"],
            data["color"], data["engineSize"], data["maxAcceleration"], convertMotorcycleTypeStrToEnum(data["motorcycleType"])
    };

    tmp.uuid = data["uuid"];
    return tmp;
}

Motorcycle Motorcycle::loadFromUUID(const std::string& uuid) {
    std::string fname = "data/vehicles/" + uuid + ".json";
    return Motorcycle::loadFromPath(fname);
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

std::string Motorcycle::toFormattedString() {
    std::string baseString = Vehicle::toFormattedString();

    baseString += "  Engine size: " + std::to_string(this->engineSize) + " CC\n";
    baseString += "  Max acceleration: " + std::to_string(this->maxAcceleration) + " m/s^2\n";
    baseString += "  Motorcycle type: " + convertMotorcycleTypeEnumToStr(this->motorcycleType) + " m/s^2\n";

    return baseString;
}