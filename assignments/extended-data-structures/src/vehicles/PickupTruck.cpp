#include "vehicles/Vehicle.hpp"
#include "vehicles/PickupTruck.hpp"
#include <fstream>
#include <filesystem>
#include <utility>

namespace fs = std::filesystem;

PickupTruck::PickupTruck(std::string name, double price, std::string manufacturer, double mileage, double horsepower,
                         double maxSpeed, double bedCapacity, double towingMaxLoad, int engineCylinderCount,
                         std::string color) : Vehicle(std::move(name), price, 4, 2, 2, 1, std::move(manufacturer), mileage, horsepower, maxSpeed, std::move(color), "pickup-truck") {
    this->bedCapacity = bedCapacity;
    this->towingMaxLoad = towingMaxLoad;
    this->engineCylinderCount = engineCylinderCount;
}

double PickupTruck::getBedCapacity() const {
    return bedCapacity;
}

double PickupTruck::getTowingMaxLoad() const {
    return towingMaxLoad;
}

double PickupTruck::getEngineCylinderCount() const {
    return engineCylinderCount;
}

double PickupTruck::approximateFuelUsageFromKm(double kilometres) {
    // Average fuel efficiency for most pickup trucks is around 8.76 km/L, we can predict that a stronger engine is
    // less efficient. It will also need more fuel to push more weight in its trunk.
    // These are just ballpark equations, fuel efficiency depends on a lot of more factors.
    double fuelEfficiency = 8.76;
    fuelEfficiency -= std::clamp((this->engineCylinderCount - 4) * 0.6, -2.0, 3.2);
    fuelEfficiency -= std::clamp((this->bedCapacity) * 0.01, 0.0, 2.0);
    fuelEfficiency -= std::clamp((this->towingMaxLoad / 2) * 0.01, 0.0, 2.0);

    // Find litres from fuel efficient and km driven using km/(km/L) = L
    double litresUsed = kilometres / fuelEfficiency;
    return litresUsed;
}

json PickupTruck::serializeToJSON() {
    json baseData = Vehicle::serializeToJSON();

    baseData["bedCapacity"] = bedCapacity;
    baseData["towingMaxLoad"] = towingMaxLoad;
    baseData["engineCylinderCount"] = engineCylinderCount;

    return baseData;
}

PickupTruck PickupTruck::deserializeFromJSON(const json &data) {
    json sanitizedData = Vehicle::getSanitizedJSON(data);

    // Ensure that all of PickupTruck's keys are there
    std::vector<std::string> requiredKeys = {"bedCapacity", "towingMaxLoad", "engineCylinderCount"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }

        sanitizedData[key] = data[key];
    }

    PickupTruck tmp{
            data["name"], data["price"], data["manufacturer"], data["mileage"], data["horsepower"], data["maxSpeed"], data["bedCapacity"], data["towingMaxLoad"], data["engineCylinderCount"], data["color"]
    };

    tmp.uuid = data["uuid"];

    return tmp;
}

PickupTruck PickupTruck::loadFromUUID(const std::string& uuid) {
    std::string fname = "data/vehicles/" + uuid + ".json";
    return PickupTruck::loadFromPath(fname);
}

PickupTruck PickupTruck::loadFromPath(const std::string& path) {
    if (!fs::exists(path)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(path);
    json importedJSON;
    file >> importedJSON;

    return PickupTruck::deserializeFromJSON(importedJSON);
}

std::string PickupTruck::toFormattedString() {
    std::string baseString = Vehicle::toFormattedString();

    baseString += "  Bed capacity: " + std::to_string(this->bedCapacity) + " kg\n";
    baseString += "  Towing max load: " + std::to_string(this->towingMaxLoad) + " kg\n";
    baseString += "  Engine cylinder count: " + std::to_string(this->engineCylinderCount) + "\n";

    return baseString;
}