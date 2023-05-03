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
     * @param trunkCapacity trunk capacity available in sedan, in kilograms
     * @param color color of sedan
     */
    Sedan(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed,
          double trunkCapacity, double engineCylinderCount, std::string color);

    /**
     * Get the max weight that can be in the trunk.
     * @return the trunk capacity in kg
     */
    double getTrunkCapacity() const;

    /**
     * Get the number of cylinders in the sedan's engine.
     * @return The number of cylinders in its engine.
     */
    double getEngineCylinderCount() const;

    /**
     * Roughly approximate the fuel usage for the vehicle to travel a certain amount of kilometres.
     * @param kilometres Number of kilometres
     * @return Fuel usage in litres
     */
    double approximateFuelUsageFromKm(double kilometres) override;

    /**
     * Serialize all the data in the class into JSON.
     * @return serialized JSON data of the class
     */
    json serializeToJSON() override;

    /**
     * Deserialize all the data from a JSON file into an instance of Sedan.
     * @param data the JSON data to deserialize
     * @return a new Sedan instance made using the JSON data.
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
     * Prepares the information within the Sedan class to be printed as a string.
     * @return formatted string of all of the info in Sedan
     */
    std::string to_formatted_string() const override;

private:
    double trunkCapacity;
    double engineCylinderCount;
};