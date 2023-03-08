#pragma once

#include <string>
#include <vector>
#include "BankAccount.hpp"
#include "Vehicle.hpp"


class VehicleDealership {
    public:
        VehicleDealership(std::string name, BankAccount* bankAccount);
        std::vector<Vehicle*> getVehicles();
        Vehicle* buyVehicleFrom(int idx, BankAccount* buyerBankAccount);
        int sellVehicleTo(Vehicle* vehicle, BankAccount* sellerBankAccount);
    private:
        std::string name;
        std::vector<Vehicle*> vehicles;
        BankAccount* bankAccount;
};