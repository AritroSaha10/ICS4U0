#include "VehicleDealership.hpp"
// #include "include/uuid_v4/uuid_v4.h.old"
#include <util.hpp>

VehicleDealership::VehicleDealership(std::string name, BankAccount &account) : bankAccount(account) {
    this->name = name;

    // UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    // this->uuid = uuidGenerator.getUUID().str();
    this->uuid = generate_uuid_v4();
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
    if (!BankAccount::makeTransaction(buyerBankAccount, &bankAccount, desiredVehicle->getPrice())) {
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
    if (!BankAccount::makeTransaction(&bankAccount, sellerBankAccount, vehiclePrice)) {
        // Transaction wasn't possible and didn't occur, don't get vehicle
        return -1;
    }

    vehicles.push_back(vehicle);
    return (int) vehicles.size() - 1;
}

int VehicleDealership::giveVehicle(Vehicle *vehicle) {
    vehicles.push_back(vehicle);
    return (int) vehicles.size() - 1;
}