#include "Person.hpp"
#include <string>
#include <chrono>
// #include "include/uuid_v4/uuid_v4.h.old"
#include "util.hpp"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
using namespace std::chrono;

Person::Person(std::string firstName, std::string middleName, std::string lastName, int birthTimestamp, double height,
               BankAccount *bankAccount) {
    this->firstName = firstName;
    this->middleName = middleName;
    this->lastName = lastName;
    this->birthTimestamp = birthTimestamp;
    this->height = height;
    this->bankAccount = bankAccount;

    // UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    // this->uuid = uuidGenerator.getUUID().str();
    this->uuid = generate_uuid_v4();
}

Person::Person(std::string firstName, std::string middleName, std::string lastName, int birthTimestamp, double height,
               BankAccount *bankAccount, std::string uuid) {
    this->firstName = firstName;
    this->middleName = middleName;
    this->lastName = lastName;
    this->birthTimestamp = birthTimestamp;
    this->height = height;
    this->bankAccount = bankAccount;

    this->uuid = uuid;
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
    double ageYears = ageSeconds / 60.0 / 60.0 / 24.0 / 365.0;

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

    /*
    // Bank account shouldn't be used by anyone else
    delete bankAccount;
    bankAccount = nullptr;
     */

    return true;
}

json Person::serializeToJSON() {
    json serialized = {};

    // Store all important info into JSON object
    serialized["uuid"] = uuid;
    serialized["firstName"] = firstName;
    serialized["middleName"] = middleName;
    serialized["lastName"] = lastName;
    serialized["birthTimestamp"] = birthTimestamp;
    serialized["height"] = height;
    serialized["bankAccount"] = bankAccount->serializeToJSON();
    for (Vehicle* vehicle : vehicles) {
        serialized["vehicles"].push_back(vehicle->serializeToJSON());
    }

    return serialized;
}

Person Person::deserializeFromJSON(const json &data) {
    // Ensure that all keys are there
    std::vector<std::string> requiredKeys = {"uuid", "firstName", "middleName", "lastName", "birthTimestamp", "height",
                                             "bankAccount", "vehicles"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw;
        }
    }

    // Make a BankAccount from the given info
    auto* tmpAccount = new BankAccount(BankAccount::deserializeFromJSON(data["bankAccount"]));

    Person tmpPerson{data["firstName"].get<std::string>(), data["middleName"].get<std::string>(),
                     data["lastName"].get<std::string>(), data["birthTimestamp"].get<int>(), data["height"].get<double>(),
                     tmpAccount, data["uuid"].get<std::string>()};

    for (const json& vehicleData : data["vehicles"].get<json>()) {
        tmpPerson.vehicles.push_back(new Vehicle(Vehicle::deserializeFromJSON(vehicleData)));
    }

    // Initialize Person using all the info
    return tmpPerson;
}

void Person::saveAsFile() {
    json serializedJSON = serializeToJSON();

    // Make data directory if needed
    if (!fs::is_directory("data") || !fs::exists("data")) { // Check if folder exists
        fs::create_directory("data");
    }

    // Make bank accounts directory if needed
    if (!fs::is_directory("data/people") || !fs::exists("data/people")) {
        fs::create_directory("data/people");
    }

    // Write data to file
    std::ofstream file("data/people/" + uuid + ".json");
    file << std::setw(4) << serializedJSON << std::endl;
    file.close();
}

Person Person::loadFromUUID(std::string uuid) {
    std::string fname = "data/people/" + uuid + ".json";
    if (!fs::exists(fname)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(fname);
    json importedJSON;
    file >> importedJSON;

    return Person::deserializeFromJSON(importedJSON);
}

Person::~Person() {
    removeBankAccount();
}