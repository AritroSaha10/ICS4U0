#pragma once

#include <string>
#include <vector>
#include "BankAccount.hpp"
#include "Vehicle.hpp"


class VehicleDealership {
    public:
        VehicleDealership(std::string name, BankAccount* bankAccount);
        std::vector<Vehicle*> getVehicles();
        Vehicle* buyVehicleTo(int idx);
        int sellVehicleTo(Vehicle* vehicle);
    private:
        std::string name;
        std::vector<Vehicle*> vehicles;
        BankAccount* bankAccount;
};