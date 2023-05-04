#include "VehicleDealership.hpp"
// #include "include/uuid_v4/uuid_v4.h.old"
#include <util.hpp>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

VehicleDealership::VehicleDealership(std::string name, BankAccount *account) {
    this->name = name;
    this->bankAccount = account;

    this->uuid = generate_uuid_v4();
}

VehicleDealership::VehicleDealership(std::string name, BankAccount *account, std::string uuid) {
    this->name = name;
    this->bankAccount = account;

    this->uuid = uuid;
}

std::string VehicleDealership::getName() const {
    return name;
}

Vehicle *VehicleDealership::buyVehicleFrom(int idx, BankAccount *buyerBankAccount) {
    if (idx > vehicles.size() - 1) {
        // Can't sell a vehicle that we don't have
        return nullptr;
    }

    Vehicle *desiredVehicle = vehicles[idx];
    // Try making the transaction
    if (!BankAccount::makeTransaction(buyerBankAccount, bankAccount, desiredVehicle->getPrice())) {
        // Transaction didn't go through (not enough money / withdraw limit)
        return nullptr;
    }

    // We no longer own this vehicle
    vehicles.erase(vehicles.begin() + idx);
    return desiredVehicle;
}

int VehicleDealership::sellVehicleTo(Vehicle *vehicle, BankAccount *sellerBankAccount) {
    if (vehicle == nullptr) {
        // No vehicle to add
        return -1;
    }

    if (std::find(vehicles.begin(), vehicles.end(), vehicle) != vehicles.end()) {
        // We already own this vehicle
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
    if (std::find(vehicles.begin(), vehicles.end(), vehicle) != vehicles.end()) {
        // We already own this vehicle
        return -1;
    }

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
    for (Vehicle *vehicle: vehicles) {
        serialized["vehicles"].push_back(vehicle->getUUID());
    }

    return serialized;
}

VehicleDealership VehicleDealership::deserializeFromJSON(const json &data, std::map<std::string, Vehicle*>& vehicleUUIDsToPointers) {
    // Ensure that all keys are there
    std::vector<std::string> requiredKeys = {"uuid", "name",
                                             "bankAccount", "vehicles"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }
    }

    // Make a BankAccount from the given info
    auto *tmpAccount = new BankAccount(BankAccount::deserializeFromJSON(data["bankAccount"]));

    VehicleDealership tmpVehicleDealership{data["name"].get<std::string>(), tmpAccount,
                                           data["uuid"].get<std::string>()};

    for (const std::string& uuid: data["vehicles"].get<json>()) {
        if (!vehicleUUIDsToPointers.contains(uuid)) {
            std::cout << "WARN: Vehicle of UUID " << uuid << " does not exist! Skipping.\n";
            continue;
        }
        tmpVehicleDealership.vehicles.push_back(vehicleUUIDsToPointers.at(uuid));
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

VehicleDealership VehicleDealership::loadFromPath(const std::string &path, std::map<std::string, Vehicle*>& vehicleUUIDsToPointers) {
    if (!fs::exists(path)) {
        // File needs to exist to read anything
        throw std::runtime_error(path + " does not exist");
    }

    std::ifstream file(path);
    json importedJSON;
    file >> importedJSON;

    return VehicleDealership::deserializeFromJSON(importedJSON, vehicleUUIDsToPointers);
}

VehicleDealership VehicleDealership::loadFromUUID(const std::string &uuid, std::map<std::string, Vehicle*>& vehicleUUIDsToPointers) {
    return VehicleDealership::loadFromPath("data/dealership/" + uuid + ".json", vehicleUUIDsToPointers);
}

std::ostream &operator<<(std::ostream &out, const VehicleDealership &obj) {
    out << "Dealership Name: " << obj.name << "\n";
    out << "  UUID: " << obj.uuid << "\n";
    out << "  # of vehicles: " << formatWithCommas(obj.vehicles.size()) << "\n";
    out << "Bank Account Info:\n" << *(obj.bankAccount) << "\n";

    return out;
}