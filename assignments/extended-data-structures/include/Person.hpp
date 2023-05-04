#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "BankAccount.hpp"
#include "include/vehicles/Vehicle.hpp"

using json = nlohmann::json;

class Vehicle;

/**
 * A class that represents a person.
 */
class Person {
public:
    /**
     * Constructor for Person, which auto-generates a UUID.
     * @param firstName first name
     * @param middleName middle name (can be empty)
     * @param lastName last name
     * @param birthTimestamp unix timestamp of person's birthday in seconds
     * @param height height in centimetres
     * @param bankAccount pointer to person's bank account
     */
    Person(std::string firstName, std::string middleName, std::string lastName, int64_t birthTimestamp, double height,
           BankAccount *bankAccount);

    /**
     * Constructor for Person, which takes in a UUID. Helpful for loading in serialized data.
     * @param firstName first name
     * @param middleName middle name (can be empty)
     * @param lastName last name
     * @param birthTimestamp unix timestamp of person's birthday in seconds
     * @param height height in centimetres
     * @param bankAccount pointer to person's bank account
     * @param uuid UUID of the instance
     */
    Person(std::string firstName, std::string middleName, std::string lastName, int64_t birthTimestamp, double height,
           BankAccount *bankAccount, std::string uuid);

    /**
     * Get the Person's first name
     * @return first name
     */
    std::string getFirstName();

    /**
     * Attempt to set the first name of the Person
     * @param newName new proposed first name
     * @return if the change was successful
     */
    bool setFirstName(std::string newName);

    /**
     * Get the Person's middle name
     * @return middle name
     */
    std::string getMiddleName();

    /**
     * Set the middle name of the Person
     * @param newName new proposed middle name
     * @return True (returns bool to match return type of other getters)
     */
    bool setMiddleName(std::string newName);

    /**
     * Get the Person's last name
     * @return last name
     */
    std::string getLastName();

    /**
     * Attempt to set the last name of the Person
     * @param newName new proposed last name
     * @return if the change was successful
     */
    bool setLastName(std::string newName);

    /**
     * Get the Person's birth unix timestamp
     * @return birth date as unix timestamp
     */
    int64_t getBirthTimestamp() const;


    bool changeHeight(double delta);

    /**
     * Calculate the Person's age in years
     * @return age in years
     */
    double getAge() const;
    /*
    bool addBankAccount(BankAccount* account); // Are these needed?
    bool removeBankAccount(); // Are these needed?
     */

    /**
     * Serialize all the data in the class into JSON.
     * @return serialized JSON data of the class
     */
    json serializeToJSON();

    /**
     * Deserialize all the data from a JSON file into an instance of Person.
     * @param data the JSON data to deserialize
     * @param vehicleUUIDsToPointers a map of UUIDs to Vehicle pointers loaded in from their folders
     * @return a new Person instance made using the JSON data.
     * @throws runtime_error if required key does not exist
     */
    static Person deserializeFromJSON(const json &data, std::map<std::string, Vehicle*>& vehicleUUIDsToPointers);

    /**
     * Save the data in the instance as a JSON file in its expected location.
     * The filename of the saved file is the UUID.
     */
    void saveAsFile();

    /**
     * Loads in a Person instance from a file given its UUID
     * @param uuid the UUID of the object
     * @return An instance of the deserialized Person from the JSON file
     * @throws runtime_error if a file with the UUID does not exist
     */
    static Person loadFromUUID(std::string uuid);

    /**
     * Loads in a Person instance from a file given its path
     * @param path the path of the serialized instance
     * @return An instance of the deserialized Person from the JSON file
     * @throws runtime_error if a file does not exist at given path
     */
    static Person loadFromPath(std::string path);

    std::vector<Vehicle *> vehicles;
    BankAccount *bankAccount;

    /**
     * Destructor for the Person.
     */
    ~Person();

    /**
     * Converts the Person into string representation.
     * @param out Output stream to feed string data into
     * @param obj Person object to use for data
     * @return Modified output stream with Person data
     */
    friend std::ostream &operator<<(std::ostream &out, const Person &obj);

    friend class Vehicle;

private:
    std::string firstName;
    std::string middleName;
    std::string lastName;
    int64_t birthTimestamp;
    double height;
    std::string uuid;
};

