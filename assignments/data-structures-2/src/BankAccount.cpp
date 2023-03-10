#include <cassert>
#include "BankAccount.hpp"
// #include "include/uuid_v4/uuid_v4.h.old"
#include <util.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

BankAccount::BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit) {
    assert(startingBalance >= minBalance && "Starting balance is higher than minimum balance");

    this->balance = startingBalance;
    this->minBalance = minBalance;
    this->withdrawLimit = withdrawLimit;
    this->depositLimit = depositLimit;

    // UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    // this->uuid = uuidGenerator.getUUID().str();
    this->uuid = generate_uuid_v4();
}

BankAccount::BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit,
                         std::string uuid) {
    assert(startingBalance >= minBalance && "Starting balance is higher than minimum balance");

    this->balance = startingBalance;
    this->minBalance = minBalance;
    this->withdrawLimit = withdrawLimit;
    this->depositLimit = depositLimit;

    this->uuid = uuid;
}

BankAccount::BankAccount(double withdrawLimit, double depositLimit) {
    this->balance = 0.0;
    this->minBalance = 0.0;
    this->withdrawLimit = withdrawLimit;
    this->depositLimit = depositLimit;

    // UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    // this->uuid = uuidGenerator.getUUID().str();
    this->uuid = generate_uuid_v4();
}

std::string BankAccount::getUUID() const {
    return uuid;
}

double BankAccount::getBalance() const {
    return balance;
}

bool BankAccount::withdraw(double amount) {
    if (!checkWithdraw(amount)) {
        // Don't allow an impossible withdraw
        return false;
    }

    balance -= amount;
    return true;
}

bool BankAccount::deposit(double amount) {
    if (!checkDeposit(amount)) {
        // Don't allow an impossible deposit
        return false;
    }

    balance += amount;
    return true;
}

bool BankAccount::makeTransaction(BankAccount *sender, BankAccount *receiver, double amount) {
    // Confirm transactions are possible
    if (!BankAccount::checkTransaction(sender, receiver, amount)) {
        std::cout << "transaction impossible\n";
        return false;
    }

    // Make the transactions
    sender->withdraw(amount);
    receiver->deposit(amount);

    return true;
}

bool BankAccount::checkTransaction(BankAccount *sender, BankAccount *receiver, double proposedAmount) {
    return sender->checkWithdraw(proposedAmount) && receiver->checkDeposit(proposedAmount);
}

bool BankAccount::checkWithdraw(double proposedAmount) const {
    return proposedAmount <= withdrawLimit && proposedAmount <= balance - minBalance;
}

bool BankAccount::checkDeposit(double proposedAmount) const {
    return proposedAmount <= depositLimit;
}

json BankAccount::serializeToJSON() {
    json serialized = {};

    // Store all important info into JSON object
    serialized["uuid"] = uuid;
    serialized["balance"] = balance;
    serialized["minBalance"] = minBalance;
    serialized["withdrawLimit"] = withdrawLimit;
    serialized["depositLimit"] = depositLimit;

    return serialized;
}

BankAccount BankAccount::deserializeFromJSON(const json &data) {
    // Ensure that all keys are there
    std::vector<std::string> requiredKeys = {"uuid", "balance", "minBalance", "withdrawLimit", "depositLimit"};
    for (const std::string &key: requiredKeys) {
        if (!data.contains(key)) {
            throw std::runtime_error(key + " does not exist in JSON");
        }
    }

    // Initialize BankAccount using all the info
    return {data["balance"].get<double>(), data["minBalance"].get<double>(), data["withdrawLimit"].get<double>(),
            data["depositLimit"].get<double>(), data["uuid"].get<std::string>()};
}

void BankAccount::saveAsFile() {
    json serializedJSON = serializeToJSON();

    // Make data directory if needed
    if (!fs::is_directory("data") || !fs::exists("data")) { // Check if folder exists
        fs::create_directory("data");
    }

    // Make bank accounts directory if needed
    if (!fs::is_directory("data/bank_accounts") || !fs::exists("data/bank_accounts")) {
        fs::create_directory("data/bank_accounts");
    }

    // Write data to file
    std::ofstream file("data/bank_accounts/" + getUUID() + ".json");
    file << std::setw(4) << serializedJSON << std::endl;
    file.close();
}

/*
BankAccount BankAccount::loadFromUUID(std::string uuid) {
    std::string fname = "data/bank_accounts/" + uuid + ".json";
    if (!fs::exists(fname)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(fname);
    json importedJSON;
    file >> importedJSON;

    return BankAccount::deserializeFromJSON(importedJSON);
}
 */

std::ostream & operator <<(std::ostream &out, const BankAccount &obj) {
    out << "  Balance: $" << formatWithCommas(obj.balance) << "\n  Minimum balance: $" << formatWithCommas(
            obj.minBalance) << "\n  Withdraw limit: $" << formatWithCommas(
            obj.withdrawLimit) << "\n  Deposit limit: $" << formatWithCommas(
            obj.depositLimit) << "\n";
    return out;
}
