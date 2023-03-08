#include <iostream>
#include "BankAccount.hpp"
#include "Person.hpp"
#include "Sedan.hpp"
#include "VehicleDealership.hpp"
#include "util.hpp"

void testSerializationBankAccount() {
    BankAccount bankAccount(100, 0, 2500, 2500);
    std::cout << "Saving this bank account: " << bankAccount.serializeToJSON() << "\n";
    std::string uuid = bankAccount.getUUID();
    bankAccount.saveAsFile();

    BankAccount newBankAccount = BankAccount::loadFromUUID(uuid);
    std::cout << "Loaded JSON: " << bankAccount.serializeToJSON() << "\n";
}

int main() {
    // testSerializationBankAccount();
    BankAccount hondaDealershipBankAccount(0, 1000000, 1000000);
    VehicleDealership hondaDealership("Honda Dealership", hondaDealershipBankAccount);
    Sedan sedan1("Honda Accord", 50000.0, "Honda", 0, 200, 300, 50, 20, "Green");
    Sedan sedan2("Honda Civic", 35042.0, "Honda", 0, 150, 175, 20, 20, "Grey");
    hondaDealership.giveVehicle(&sedan1);
    hondaDealership.giveVehicle(&sedan2);

    // std::cout << sedan1.serializeToJSON() << "\n\n";

    for (Vehicle* vehicle : hondaDealership.getVehicles()) {
        std::cout << vehicle->getName() << " - $" << vehicle->getPrice() << " - " << vehicle->serializeToJSON() << "\n\n";
    }

    BankAccount person1Bank(0, 60000, 150000);
    Person p1("Aritro", "", "Saha", 1158019200, 167.64, &person1Bank);
    person1Bank.deposit(100000);

    Vehicle* boughtVehicle = hondaDealership.buyVehicleFrom(0, p1.bankAccount);
    if (boughtVehicle == nullptr) {
        std::cout << "Something went wrong when buying the vehicle\n";
        return 1;
    }
    p1.vehicles.push_back(boughtVehicle);

    for (Vehicle* vehicle : p1.vehicles) {
        std::cout << vehicle->getName() << " - $" << vehicle->getPrice() << " - " << vehicle->serializeToJSON() << "\n\n";
    }


    return 0;
}