#pragma once
#include <vector>
#include "Vehicle.hpp"

/**
 * A class that represents a Sedan, a type of vehicle. As such, it inherits from Vehicle.
 */
class Sedan : public Vehicle {
public:
    /**
     * Constructor for Sedan, inherits from Vehicle.
     * @param name name of sedan
     * @param price price of sedan, in dollars
     * @param manufacturer manufacturer of sedan
     * @param mileage mileage of sedan, in kilometres
     * @param horsepower horsepower of sedan
     * @param maxSpeed max speed of sedan, in km/h
     * @param trunkCapacity trunk capacity available in sedan, in litres
     * @param towingMaxLoad max weight that can be towed, in kg
     * @param color color of sedan
     */
    Sedan(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed, double trunkCapacity, double towingMaxLoad, std::string color);

    /**
     * Get the trunk capacity.
     * @return the trunk capacity
     */
    double getTrunkCapacity() const;

    /**
     * Get the max weight that can be towed by the sedan.
     * @return The max towable weight in kilograms.
     */
    double getTowingMaxLoad() const;

    /**
     * Serialize all the data in the class into JSON.
     * @return serialized JSON data of the class
     */
    json serializeToJSON() override;

    /**
     * Deserialize all the data from a JSON file into an instance of VehicleDealership.
     * @param data the JSON data to deserialize
     * @return a new VehicleDealership instance made using the JSON data.
     * @throws runtime_error if required key does not exist
     */
    // static Sedan deserializeFromJSON(const json &data);

    /**
     * Save the data in the instance as a JSON file in its expected location.
     * The filename of the saved file is the UUID.
     */
    void saveAsFile() override;

    // static Sedan loadFromUUID(std::string uuid);
private:
    double trunkCapacity;
    double towingMaxLoad;
};