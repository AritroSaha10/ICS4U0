#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "BankAccount.hpp"
#include "Vehicle.hpp"

using json = nlohmann::json;

class Vehicle;

class Person {
public:
    // DELETE THE BANKACCOUNT PART OF THESE CONSTRUCTORS
    // MAKE THE BANK ACCOUNT AND MANAGE ITS POINTERS INTERNALLY
    // FIXES MANY PROBLEMS AT ONCE
    Person(std::string firstName, std::string middleName, std::string lastName, int birthTimestamp, double height, BankAccount* bankAccount);
    Person(std::string firstName, std::string middleName, std::string lastName, int birthTimestamp, double height, BankAccount* bankAccount, std::string uuid);
    std::string getFirstName();
    bool setFirstName(std::string newName);
    std::string getMiddleName();
    bool setMiddleName(std::string newName);
    std::string getLastName();
    bool setLastName(std::string newName);
    int getBirthTimestamp() const;
    bool changeHeight(double delta);
    double getAge() const;
    bool addBankAccount(BankAccount* account); // Are these needed?
    bool removeBankAccount(); // Are these needed?

    json serializeToJSON();
    static Person deserializeFromJSON(const json &data);
    std::vector<Vehicle*> vehicles;
    BankAccount* bankAccount;
    ~Person();

    friend class Vehicle;

private:
    std::string firstName;
    std::string middleName;
    std::string lastName;
    int64_t birthTimestamp;
    double height;
    std::string uuid;
};

