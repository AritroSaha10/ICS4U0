#pragma once

#include <string>
#include <vector>
#include "BankAccount.hpp"
#include "Vehicle.hpp"
// #include "include/uuid_v4/uuid_v4.h.old"
#include "util.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class VehicleDealership {
    public:
        VehicleDealership(std::string name, BankAccount *account);
        VehicleDealership(std::string name, BankAccount *account, std::string uuid);

        std::string getName() const;
        std::vector<Vehicle*> getVehicles();
        Vehicle* buyVehicleFrom(int idx, BankAccount* buyerBankAccount);
        int sellVehicleTo(Vehicle* vehicle, BankAccount* sellerBankAccount);
        int giveVehicle(Vehicle* vehicle);

        json serializeToJSON();
        static VehicleDealership deserializeFromJSON(const json &data);
        void saveAsFile();
        static VehicleDealership loadFromUUID(std::string uuid);
        static VehicleDealership loadFromPath(std::string path);

        std::vector<Vehicle*> vehicles;
    private:
        std::string name;

        BankAccount *bankAccount;
        std::string uuid;
};