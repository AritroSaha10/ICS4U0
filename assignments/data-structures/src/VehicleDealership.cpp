#include "VehicleDealership.hpp"

VehicleDealership::VehicleDealership(std::string name, BankAccount *bankAccount) {
    this->name = name;
    this->bankAccount = bankAccount;
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