#include <cassert>
#include "BankAccount.hpp"

BankAccount::BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit) {
    assert(startingBalance >= minBalance && "Starting balance is higher than minimum balance");

    this->balance = startingBalance;
    this->minBalance = minBalance;
    this->withdrawLimit = withdrawLimit;
    this->depositLimit = depositLimit;
}

BankAccount::BankAccount(double minBalance, double withdrawLimit, double depositLimit) {
    this->balance = 0.0;
    this->minBalance = minBalance;
    this->withdrawLimit = withdrawLimit;
    this->depositLimit = depositLimit;
}

double BankAccount::getBalance() const {
    return balance;
}

bool BankAccount::withdraw(double amount) {
    if (amount > withdrawLimit || amount > balance - minBalance) {
        // Don't allow an impossible withdraw
        return false;
    }

    balance -= amount;
    return true;
}

bool BankAccount::deposit(double amount) {
    if (amount > depositLimit) {
        // Don't allow an impossible deposit
        return false;
    }

    balance += amount;
    return true;
}