#pragma once

#include <string>
#include <vector>
#include "BankAccount.hpp"
#include "Vehicle.hpp"
#include "util.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * A class for a vehicle dealership, which is a company that can own, buy, and sell cars
 */
class VehicleDealership {
public:
    /**
     * Constructor for VehicleDealership, which auto-generates a UUID.
     * @param name name of the dealership
     * @param account bank account of the dealership
     */
    VehicleDealership(std::string name, BankAccount *account);

    /**
     * Constructor for VehicleDealership, which takes in a UUID. Helpful for loading in data from save state.
     * @param name
     * @param account
     * @param uuid
     */
    VehicleDealership(std::string name, BankAccount *account, std::string uuid);

    /**
     * Get the name of the dealership.
     * @return The name of the dealership.
     */
    std::string getName() const;

    /**
     * Attempt to buy a vehicle from the dealership.
     * @param idx the index in the vehicle list of which vehicle to buy
     * @param buyerBankAccount the bank account of the buyer of the vehicle
     * @return A pointer to the Vehicle desired, will be null if vehicles[idx] does not exist
     */
    Vehicle *buyVehicleFrom(int idx, BankAccount *buyerBankAccount);

    /**
     * Attempt to sell a vehicle to the dealership.
     * @param vehicle pointer to the Vehicle to sell to dealership
     * @param sellerBankAccount the seller of the Vehicle
     * @return The index of the new vehicle in the vehicles list, returns -1 if it was unsuccessful
     */
    int sellVehicleTo(Vehicle *vehicle, BankAccount *sellerBankAccount);

    /**
     * Attempt to add a vehicle to the dealership
     * @param vehicle pointer to vehicle to give to the dealership (no payment)
     * @return Index of the new vehicle in vehicles list, returns -1 if dealership already owns it
     */
    int giveVehicle(Vehicle *vehicle);

    /**
     * Serialize all the data in the class into JSON.
     * @return serialized JSON data of the class
     */
    json serializeToJSON();

    /**
     * Deserialize all the data from a JSON file into an instance of VehicleDealership.
     * @param data the JSON data to deserialize
     * @return a new VehicleDealership instance made using the JSON data.
     * @throws runtime_error if required key does not exist
     */
    static VehicleDealership deserializeFromJSON(const json &data);

    /**
     * Save the data in the instance as a JSON file in its expected location.
     * The filename of the saved file is the UUID.
     */
    void saveAsFile();

    /**
     * Loads in a VehicleDealership instance from a file given its UUID
     * @param uuid the UUID of the object
     * @return An instance of the deserialized VehicleDealership from the JSON file
     * @throws runtime_error if a file with the UUID does not exist
     */
    static VehicleDealership loadFromUUID(const std::string& uuid);

    /**
     * Loads in a VehicleDealership instance from a file given its path
     * @param path the path of the serialized instance
     * @return An instance of the deserialized VehicleDealership from the JSON file
     * @throws runtime_error if a file does not exist at given path
     */
    static VehicleDealership loadFromPath(const std::string& path);

    /**
     * Convert the data in the instance into a string
     * @param out the current output stream to add onto
     * @param obj reference to VehicleDealership to get data from
     * @return the manipulated output stream with the Vehicle data
     */
    friend std::ostream &operator<<(std::ostream &out, const VehicleDealership &obj);

    std::vector<Vehicle *> vehicles;
private:
    std::string name;

    BankAccount *bankAccount;
    std::string uuid;
};