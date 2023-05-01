#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * A class for a bank account, storing data such as a balance and constants such as minimum balance or limits.
 */
class BankAccount {
public:
    /**
     * Constructor for BankAccount, which auto-generates a UUID.
     * @param startingBalance the starting balance
     * @param minBalance the minimum balance at any time, must be lower than startingBalance
     * @param withdrawLimit the maximum amount available to withdraw at once
     * @param depositLimit the maximum amount available to deposit at once
     */
    BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit);

    /**
     * Constructor for BankAccount, which takes in a UUID. Helpful for loading data in from save state.
     * @param startingBalance the starting balance
     * @param minBalance the minimum balance at any time, must be lower than startingBalance
     * @param withdrawLimit the maximum amount available to withdraw at once
     * @param depositLimit the maximum amount available to deposit at once
     * @param uuid the UUID of the object, used in saving and identifying instances
     */
    BankAccount(double startingBalance, double minBalance, double withdrawLimit, double depositLimit, std::string uuid);

    /**
     * Constructor for BankAccount, which assumes a starting and minimum balance of 0 and auto-generates a UUID.
     * @param withdrawLimit the maximum amount available to withdraw at once
     * @param depositLimit the maximum amount available to deposit at once
     */
    BankAccount(double withdrawLimit, double depositLimit);

    /**
     * Gets the current bank balance
     * @return current bank balance
     */
    double getBalance() const;

    /**
     * Gets the UUID of the instance
     * @return UUID of instance
     */
    std::string getUUID() const;

    /**
     * Attempt to withdraw a certain amount of money from the account
     * @param amount the amount of money to withdraw
     * @return true if the withdraw was successful, false if it was not (balance too low, exceeded withdraw limit)
     */
    bool withdraw(double amount);

    /**
     * Attempt to deposit a certain amount of money into the account
     * @param amount the amount of money to withdraw
     * @return true if the deposit was successful, false if it was not (exceeded deposit limit)
     */
    bool deposit(double amount);

    /**
     * Check whether a withdraw is possible.
     * @param proposedAmount the proposed amount to withdraw
     * @return true if it is possible, false if it is not
     */
    bool checkWithdraw(double proposedAmount) const;

    /**
     * Check whether a deposit is possible.
     * @param proposedAmount the proposed amount to withdraw
     * @return true if it is possible, false if it is not
     */
    bool checkDeposit(double proposedAmount) const;

    /**
     * Attempt to make a transaction between two parties with bank accounts.
     * @param sender the account sending the money
     * @param receiver the account receiving the money
     * @param amount the amount to transfer between both accounts
     * @return true if the transaction was successful, false if it was not
     */
    static bool makeTransaction(BankAccount *sender, BankAccount *receiver, double amount);

    /**
     * Check whether a transaction between two parties with bank accounts is possible.
     * @param sender the account that would send the money
     * @param receiver the account that would receive the money
     * @param proposedAmount the amount that would be transferred between both accounts
     * @return true if the transaction is possible, false if it is not
     */
    static bool checkTransaction(BankAccount *sender, BankAccount *receiver, double proposedAmount);

    /**
     * Serialize all the data in the class into JSON.
     * @return serialized JSON data of the class
     */
    json serializeToJSON();

    /**
     * Deserialize all the data from a JSON file into an instance of BankAccount.
     * @param data the JSON data to deserialize
     * @return a new BankAccount instance made using the JSON data.
     * @throws runtime_error if required key does not exist
     */
    static BankAccount deserializeFromJSON(const json &data);

    /**
     * Save the data in the instance as a JSON file in its expected location.
     * The filename of the saved file is the UUID.
     */
    void saveAsFile();

    /**
     * Convert the data in the instance into a string
     * @param out the current output stream to add onto
     * @param obj reference to BankAccount to get data from
     * @return the manipulated output stream with the BankAccount data
     */
    friend std::ostream &operator<<(std::ostream &out, const BankAccount &obj);

private:
    double balance;
    double minBalance;
    double withdrawLimit;
    double depositLimit;
    std::string uuid;
};