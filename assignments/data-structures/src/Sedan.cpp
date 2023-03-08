#include "Sedan.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

Sedan::Sedan(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed,
             double trunkCapacity, double towingMaxLoad, std::string color) : Vehicle(name, price, 4, 4, 5, 4, manufacturer, mileage, horsepower, maxSpeed, color) {
    this->trunkCapacity = trunkCapacity;
    this->towingMaxLoad = towingMaxLoad;
}

double Sedan::getTrunkCapacity() const {
    return trunkCapacity;
}

double Sedan::getTowingMaxLoad() const {
    return towingMaxLoad;
}

json Sedan::serializeToJSON() {
    json vehicleSerialized = Vehicle::serializeToJSON();
    vehicleSerialized["trunkCapacity"] = trunkCapacity;
    vehicleSerialized["towingMaxLoad"] = towingMaxLoad;

    return vehicleSerialized;
}

Sedan Sedan::deserializeFromJSON(const json &data) {
    // Ensure that all keys are there
    std::vector<std::string> requiredKeys = {"uuid", "name", "price", "wheels", "doors", "seats",
                                             "maxPassengers", "maxSpeed", "manufacturer", "mileage", "horsepower",
                                             "started", "color", "trunkCapacity", "towingMaxLoad"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }
    }

    // Initialize Vehicle using all the info
    return {data["name"].get<std::string>(), data["price"].get<double>(),
            data["manufacturer"].get<std::string>(), data["mileage"].get<double>(), data["horsepower"].get<double>(),
            data["maxSpeed"].get<double>(), data["trunkCapacity"].get<double>(), data["towingMaxLoad"].get<double>(), data["color"].get<std::string>()};
}

void Sedan::saveAsFile() {
    json serializedJSON = serializeToJSON();

    // Make data directory if needed
    if (!fs::is_directory("data") || !fs::exists("data")) { // Check if folder exists
        fs::create_directory("data");
    }

    // Make bank accounts directory if needed
    if (!fs::is_directory("data/vehicle") || !fs::exists("data/vehicle")) {
        fs::create_directory("data/vehicle");
    }

    // Write data to file
    std::ofstream file("data/vehicle/" + uuid + ".json");
    file << std::setw(4) << serializedJSON << std::endl;
    file.close();
}

Sedan Sedan::loadFromUUID(std::string uuid) {
    std::string fname = "data/bank_accounts/" + uuid + ".json";
    if (!fs::exists(fname)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(fname);
    json importedJSON;
    file >> importedJSON;

    return Sedan::deserializeFromJSON(importedJSON);
}