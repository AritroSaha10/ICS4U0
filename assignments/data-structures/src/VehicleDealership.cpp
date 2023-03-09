#include "VehicleDealership.hpp"
// #include "include/uuid_v4/uuid_v4.h.old"
#include <util.hpp>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;


VehicleDealership::VehicleDealership(std::string name, BankAccount *account) {
    this->name = name;
    this->bankAccount = account;

    // UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    // this->uuid = uuidGenerator.getUUID().str();
    this->uuid = generate_uuid_v4();
}

VehicleDealership::VehicleDealership(std::string name, BankAccount *account, std::string uuid) {
    this->name = name;
    this->bankAccount = account;

    // UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    // this->uuid = uuidGenerator.getUUID().str();
    this->uuid = uuid;
}

std::string VehicleDealership::getName() const {
    return name;
}

std::vector<Vehicle*> VehicleDealership::getVehicles() {
    return vehicles;
}

Vehicle* VehicleDealership::buyVehicleFrom(int idx, BankAccount* buyerBankAccount) {
    if (idx > vehicles.size() - 1) {
        // Can't sell a vehicle that we don't have
        return nullptr;
    }

    Vehicle* desiredVehicle = vehicles[idx];
    // Try making the transaction
    if (!BankAccount::makeTransaction(buyerBankAccount, bankAccount, desiredVehicle->getPrice())) {
        // Transaction didn't go through (not enough money / withdraw limit)
        return nullptr;
    }

    // We no longer own this vehicle
    vehicles.erase(vehicles.begin() + idx);
    return desiredVehicle;
}

int VehicleDealership::sellVehicleTo(Vehicle* vehicle, BankAccount* sellerBankAccount) {
    if (vehicle == nullptr) {
        // No vehicle to add
        return -1;
    }

    // Try making the transaction
    double vehiclePrice = vehicle->getPrice();
    if (!BankAccount::makeTransaction(bankAccount, sellerBankAccount, vehiclePrice)) {
        // Transaction wasn't possible and didn't occur, don't get vehicle
        return -1;
    }

    vehicles.push_back(vehicle);
    return (int) vehicles.size() - 1;
}

int VehicleDealership::giveVehicle(Vehicle *vehicle) {
    // MAKE SURE NO DUPLICATES WHENEVER WE'RE BEING SOLD OR GETTING
    vehicles.push_back(vehicle);
    return (int) vehicles.size() - 1;
}

json VehicleDealership::serializeToJSON() {
    json serialized = {};

    // Store all important info into JSON object
    serialized["uuid"] = uuid;
    serialized["name"] = name;
    serialized["bankAccount"] = bankAccount->serializeToJSON();
    serialized["vehicles"] = json::array();
    for (Vehicle* vehicle : vehicles) {
        serialized["vehicles"].push_back(vehicle->serializeToJSON());
    }

    return serialized;
}

VehicleDealership VehicleDealership::deserializeFromJSON(const json &data) {
    // Ensure that all keys are there
    std::vector<std::string> requiredKeys = {"uuid", "name",
                                             "bankAccount", "vehicles"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }
    }

    // Make a BankAccount from the given info
    auto* tmpAccount = new BankAccount(BankAccount::deserializeFromJSON(data["bankAccount"]));

    VehicleDealership tmpVehicleDealership{data["name"].get<std::string>(), tmpAccount, data["uuid"].get<std::string>()};

    for (const json& vehicleData : data["vehicles"].get<json>()) {
        tmpVehicleDealership.vehicles.push_back(new Vehicle(Vehicle::deserializeFromJSON(vehicleData)));
    }

    // Initialize VehicleDealership using all the info
    return tmpVehicleDealership;
}

void VehicleDealership::saveAsFile() {
    json serializedJSON = serializeToJSON();

    // Make data directory if needed
    if (!fs::is_directory("data") || !fs::exists("data")) { // Check if folder exists
        fs::create_directory("data");
    }

    // Make bank accounts directory if needed
    if (!fs::is_directory("data/vehicle-dealership") || !fs::exists("data/vehicle-dealership")) {
        fs::create_directory("data/vehicle-dealership");
    }

    // Write data to file
    std::ofstream file("data/vehicle-dealership/" + uuid + ".json");
    file << std::setw(4) << serializedJSON << std::endl;
    file.close();
}

VehicleDealership VehicleDealership::loadFromPath(std::string path) {
    if (!fs::exists(path)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(path);
    json importedJSON;
    file >> importedJSON;

    return VehicleDealership::deserializeFromJSON(importedJSON);
}

VehicleDealership VehicleDealership::loadFromUUID(std::string uuid) {
    return VehicleDealership::loadFromPath("data/dealership/" + uuid + ".json");
}