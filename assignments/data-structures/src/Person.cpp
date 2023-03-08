#include "Person.hpp"
#include <string>
#include <chrono>

using namespace std::chrono;

Person::Person(std::string firstName, std::string middleName, std::string lastName, int birthTimestamp, double height) {
    this->firstName = firstName;
    this->middleName = middleName;
    this->lastName = lastName;
    this->birthTimestamp = birthTimestamp;
    this->height = height;
}

Person::Person(std::string firstName, std::string middleName, std::string lastName, int birthTimestamp, double height, BankAccount* bankAccount) {
    this->firstName = firstName;
    this->middleName = middleName;
    this->lastName = lastName;
    this->birthTimestamp = birthTimestamp;
    this->height = height;
    this->bankAccount = bankAccount;
}

std::string Person::getFirstName() {
    return firstName;
}

bool Person::setFirstName(std::string newName) {
    if (newName.length() == 0) {
        // Ensure that the name is valid
        return false;
    }

    this->firstName = newName;
    return true;
}

std::string Person::getMiddleName() {
    return middleName;
}

bool Person::setMiddleName(std::string newName) {
    this->middleName = newName;
    return true;
}

std::string Person::getLastName() {
    return lastName;
}

bool Person::setLastName(std::string newName) {
    if (newName.length() == 0) {
        // Ensure that the name is valid
        return false;
    }

    this->lastName = newName;
    return true;
}

int Person::getBirthTimestamp() const {
    return birthTimestamp;
}

bool Person::changeHeight(double delta) {
    if (height + delta <= 0) {
        // Ensure that the new name is valid
        return false;
    }

    height += delta;
    return true;
}

double Person::getAge() const {
    int64_t currTimeSeconds = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    int64_t ageSeconds = currTimeSeconds - birthTimestamp;
    double ageYears = ageSeconds / 60 / 60 / 24 / 365;

    return ageYears;
}

bool Person::addBankAccount(BankAccount *account) {
    if (bankAccount != nullptr) {
        // Bank account already exists, don't allow overwriting
        return false;
    }

    bankAccount = account;
    return true;
}

bool Person::removeBankAccount() {
    if (bankAccount == nullptr) {
        // Bank account doesn't exist, can't remove nothing
        return false;
    }

    // Bank account shouldn't be used by anyone else
    delete bankAccount;
    bankAccount = nullptr;

    return true;
}