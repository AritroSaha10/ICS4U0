#pragma once

class BankAccount {
    public:
        BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit);
        BankAccount(double minBalance, double withdrawLimit, double depositLimit);
        double getBalance() const;
        bool withdraw(double amount);
        bool deposit(double amount);
    private:
        double balance;
        double minBalance;
        double withdrawLimit;
        double depositLimit;
};