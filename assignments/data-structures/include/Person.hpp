#pragma once
#include <string>
#include <vector>
#include "BankAccount.hpp"
#include "Vehicle.hpp"

class Person {
public:
    Person(std::string firstName, std::string middleName, std::string lastName, int birthTimestamp, double height);
    Person(std::string firstName, std::string middleName, std::string lastName, int birthTimestamp, double height, BankAccount* bankAccount);
    std::string getFirstName();
    bool setFirstName(std::string newName);
    std::string getMiddleName();
    bool setMiddleName(std::string newName);
    std::string getLastName();
    bool setLastName(std::string newName);
    int getBirthTimestamp() const;
    bool changeHeight(double delta);
    double getAge() const;
    bool addBankAccount(BankAccount* account);
    bool removeBankAccount();
    std::string serializeToJSON();

    friend class Vehicle;

private:
    std::string firstName;
    std::string middleName;
    std::string lastName;
    int64_t birthTimestamp;
    double height;
    BankAccount* bankAccount;
};

