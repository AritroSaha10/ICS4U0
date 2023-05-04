#pragma once
#include <vector>
#include "Vehicle.hpp"

/**
 * A class that represents a Sedan, a type of vehicle. As such, it inherits from Vehicle.
 */
class PickupTruck : public Vehicle {
public:
    /**
     * Constructor for PickupTruck, inherits from Vehicle.
     * @param name name of pickup
     * @param price price of pickup, in dollars
     * @param manufacturer manufacturer of pickup
     * @param mileage mileage of pickup, in kilometres
     * @param horsepower horsepower of pickup
     * @param maxSpeed max speed of pickup, in km/h
     * @param bedCapacity trunk bed capacity available in pickup, in kg
     * @param towingMaxLoad max weight that can be towed, in kg
     * @param engineCylinderCount the number of cylinders in the pickup's engine
     * @param color color of pickup
     */
    PickupTruck(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed, double bedCapacity, double towingMaxLoad, int engineCylinderCount, std::string color);

    /**
     * Get the max weight that can be in the trunk.
     * @return the trunk capacity in kg
     */
    double getBedCapacity() const;

    /**
     * Get the max weight that can be towed by the pickup truck.
     * @return The max towable weight in kilograms.
     */
    double getTowingMaxLoad() const;

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
     * Deserialize all the data from a JSON file into an instance of PickupTruck.
     * @param data the JSON data to deserialize
     * @return a new PickupTruck instance made using the JSON data.
     * @throws runtime_error if required key does not exist
     */
    static PickupTruck deserializeFromJSON(const json &data);

    /**
     * Loads in a PickupTruck instance from a file given its UUID
     * @param uuid the UUID of the object
     * @return An instance of the deserialized PickupTruck from the JSON file
     * @throws runtime_error if a file with the UUID does not exist
     */
    static PickupTruck loadFromUUID(const std::string& uuid);

    /**
     * Loads in a PickupTruck instance from a file given its path
     * @param path the path of the serialized instance
     * @return An instance of the deserialized PickupTruck from the JSON file
     * @throws runtime_error if a file does not exist at given path
     */
    static PickupTruck loadFromPath(const std::string& path);

    /**
     * Prepares the information within the PickupTruck class to be printed as a string.
     * @return formatted string of all of the info in PickupTruck
     */
    std::string to_formatted_string() override;
private:
    double bedCapacity;
    double towingMaxLoad;
    int engineCylinderCount;
};