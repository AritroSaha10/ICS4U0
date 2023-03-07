#pragma once
#include <string>
#include <vector>
#include "BankAccount.hpp"
#include "Vehicle.hpp"

class Person {
public:
    Person();
    std::string getFirstName();
    std::string getMiddleName();
    std::string getLastName();

private:
    std::string firstName;
    std::string middleName;
    std::string lastName;
    int birthTimestamp;
    double height;
    BankAccount* bankAccount;
    std::vector<Vehicle*> vehicles;
};

