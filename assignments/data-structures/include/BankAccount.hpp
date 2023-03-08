#pragma once

class BankAccount {
    public:
        BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit);
        BankAccount(double minBalance, double withdrawLimit, double depositLimit);
        double getBalance() const;
        bool withdraw(double amount);
        bool deposit(double amount);
        bool checkWithdraw(double proposedAmount) const;
        bool checkDeposit(double proposedAmount);
        static bool makeTransaction(BankAccount* sender, BankAccount* receiver, double amount);
        static bool checkTransaction(BankAccount* sender, BankAccount* receiver, double proposedAmount);
    private:
        double balance;
        double minBalance;
        double withdrawLimit;
        double depositLimit;
};