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
    static Sedan deserializeFromJSON(const json &data);

    /**
     * Loads in a Sedan instance from a file given its UUID
     * @param uuid the UUID of the object
     * @return An instance of the deserialized Sedan from the JSON file
     * @throws runtime_error if a file with the UUID does not exist
     */
    static Sedan loadFromUUID(std::string uuid);

    /**
     * Loads in a Sedan instance from a file given its path
     * @param path the path of the serialized instance
     * @return An instance of the deserialized Sedan from the JSON file
     * @throws runtime_error if a file does not exist at given path
     */
    static Sedan loadFromPath(std::string path);

    /**
     * Convert the data in the instance into a string
     * @param out the current output stream to add onto
     * @param obj reference to Sedan to get data from
     * @return the manipulated output stream with the Vehicle data
     */
    friend std::ostream &operator<<(std::ostream &out, const Sedan &obj);
private:
    double trunkCapacity;
    double towingMaxLoad;
};