#pragma once

#include <string>
#include <vector>
#include "BankAccount.hpp"
#include "Vehicle.hpp"
// #include "include/uuid_v4/uuid_v4.h.old"
#include "util.hpp"

class VehicleDealership {
    public:
        VehicleDealership(std::string name, BankAccount &bankAccount);
        std::vector<Vehicle*> getVehicles();
        Vehicle* buyVehicleFrom(int idx, BankAccount* buyerBankAccount);
        int sellVehicleTo(Vehicle* vehicle, BankAccount* sellerBankAccount);
        int giveVehicle(Vehicle* vehicle);
    private:
        std::string name;
        std::vector<Vehicle*> vehicles;
        BankAccount &bankAccount;
        std::string uuid;
};