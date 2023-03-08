#pragma once
#include <string>
#include <vector>
#include "BankAccount.hpp"

class Person {
public:
    Person();
    std::string getFirstName();
    bool setFirstName(std::string newName);
    std::string getMiddleName();
    bool setMiddleName(std::string newName);
    std::string getLastName();
    bool setLastName(std::string newName);
    int getBirthTimestamp();
    bool changeHeight(double delta);
    double getAge();
    bool addBankAccount(BankAccount* account);
    bool removeBankAccount();
    std::string serializeToJSON();

private:
    std::string firstName;
    std::string middleName;
    std::string lastName;
    int birthTimestamp;
    double height;
    BankAccount* bankAccount;
};

