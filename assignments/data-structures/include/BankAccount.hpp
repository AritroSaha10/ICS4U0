#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class BankAccount {
    public:
        BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit);
        BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit, std::string uuid);
        BankAccount(double minBalance, double withdrawLimit, double depositLimit);
        double getBalance() const;
        std::string getUUID() const;
        bool withdraw(double amount);
        bool deposit(double amount);
        bool checkWithdraw(double proposedAmount) const;
        bool checkDeposit(double proposedAmount) const;
        static bool makeTransaction(BankAccount* sender, BankAccount* receiver, double amount);
        static bool checkTransaction(BankAccount* sender, BankAccount* receiver, double proposedAmount);
        json serializeToJSON();
        static BankAccount deserializeFromJSON(const json &data);
        void saveAsFile();
        static BankAccount loadFromUUID(std::string uuid);
    private:
        double balance;
        double minBalance;
        double withdrawLimit;
        double depositLimit;
        std::string uuid;
};