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
    // MAKE SURE TO MAKE THE BANK ACCOUNT IN THE INITALIZATION STATEMENT
    // AND ONLY MANAGE THE POINTER IN THIS CLASS
    Person(std::string firstName, std::string middleName, std::string lastName, int64_t birthTimestamp, double height, BankAccount* bankAccount);
    Person(std::string firstName, std::string middleName, std::string lastName, int64_t birthTimestamp, double height, BankAccount* bankAccount, std::string uuid);
    std::string getFirstName();
    bool setFirstName(std::string newName);
    std::string getMiddleName();
    bool setMiddleName(std::string newName);
    std::string getLastName();
    bool setLastName(std::string newName);
    int64_t getBirthTimestamp() const;
    bool changeHeight(double delta);
    double getAge() const;
    /*
    bool addBankAccount(BankAccount* account); // Are these needed?
    bool removeBankAccount(); // Are these needed?
     */

    json serializeToJSON();
    static Person deserializeFromJSON(const json &data);
    void saveAsFile();
    static Person loadFromUUID(std::string uuid);
    static Person loadFromPath(std::string path);

    std::vector<Vehicle*> vehicles;
    BankAccount* bankAccount;
    ~Person();

    friend std::ostream & operator <<(std::ostream &out, const Person &obj);
    friend class Vehicle;

private:
    std::string firstName;
    std::string middleName;
    std::string lastName;
    int64_t birthTimestamp;
    double height;
    std::string uuid;
};

