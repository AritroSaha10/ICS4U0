#include "vehicles/Vehicle.hpp"
#include "vehicles/Sedan.hpp"
#include <fstream>
#include <filesystem>
#include <utility>

namespace fs = std::filesystem;

Sedan::Sedan(std::string name, double price, std::string manufacturer, double mileage, double horsepower,
             double maxSpeed, double trunkCapacity, int engineCylinderCount,
             std::string color) : Vehicle(std::move(name), price, 4, 4, 5, 4, std::move(manufacturer), mileage, horsepower,
                                          maxSpeed, std::move(color), "sedan") {
    this->trunkCapacity = trunkCapacity;
    this->engineCylinderCount = engineCylinderCount;
}

double Sedan::getTrunkCapacity() const {
    return trunkCapacity;
}

double Sedan::getEngineCylinderCount() const {
    return engineCylinderCount;
}

double Sedan::approximateFuelUsageFromKm(double kilometres) {
    // Average fuel efficiency for most sedans is around 13.6 km/L, we can predict that a stronger engine is
    // less efficient. It will also need more fuel to push more weight in its trunk.
    // These are just ballpark equations, fuel efficiency depends on a lot of more factors.
    double fuelEfficiency = 13.6;
    fuelEfficiency -= std::clamp((this->engineCylinderCount - 4) * 0.6, -2.0, 3.2);
    fuelEfficiency -= std::clamp((this->trunkCapacity) * 0.01, 0.0, 2.0);

    // Find litres from fuel efficient and km driven using km/(km/L) = L
    double litresUsed = kilometres / fuelEfficiency;
    return litresUsed;
}

json Sedan::serializeToJSON() {
    json baseData = Vehicle::serializeToJSON();

    baseData["trunkCapacity"] = trunkCapacity;
    baseData["engineCylinderCount"] = engineCylinderCount;

    return baseData;
}

Sedan Sedan::deserializeFromJSON(const json &data) {
    json sanitizedData = Vehicle::getSanitizedJSON(data);

    // Ensure that all of Sedan's keys are there
    std::vector<std::string> requiredKeys = {"trunkCapacity", "engineCylinderCount"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }

        sanitizedData[key] = data[key];
    }

    Sedan tmp{
            data["name"], data["price"], data["manufacturer"], data["mileage"], data["horsepower"], data["maxSpeed"],
            data["trunkCapacity"], data["engineCylinderCount"], data["color"]
    };

    tmp.uuid = data["uuid"];
    return tmp;
}

Sedan Sedan::loadFromUUID(const std::string& uuid) {
    std::string fname = "data/vehicles/" + uuid + ".json";
    return Sedan::loadFromPath(fname);
}

Sedan Sedan::loadFromPath(const std::string& path) {
    if (!fs::exists(path)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(path);
    json importedJSON;
    file >> importedJSON;

    return Sedan::deserializeFromJSON(importedJSON);
}

std::string Sedan::to_formatted_string() {
    std::string baseString = Vehicle::to_formatted_string();

    baseString += "  Trunk capacity: " + std::to_string(this->trunkCapacity) + " kg\n";
    baseString += "  Engine cylinder count: " + std::to_string(this->engineCylinderCount) + "\n";

    return baseString;
}