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

bool BankAccount::makeTransaction(BankAccount* sender, BankAccount* receiver, double amount) {
    // Confirm transactions are possible
    if (!BankAccount::checkTransaction(sender, receiver, amount)) {
        return false;
    }

    // Make the transactions
    sender->withdraw(amount);
    receiver->deposit(amount);

    return true;
}

bool BankAccount::checkTransaction(BankAccount* sender, BankAccount* receiver, double proposedAmount) {
    return sender->checkWithdraw(proposedAmount) && receiver->checkDeposit(proposedAmount);
}

bool BankAccount::checkWithdraw(double proposedAmount) const {
    return proposedAmount <= withdrawLimit && proposedAmount <= balance - minBalance;
}

bool BankAccount::checkDeposit(double proposedAmount) {
    return proposedAmount > depositLimit;
}