#pragma once
#include "Vehicle.hpp"

enum MOTORCYCLE_TYPE {
    SPORT,
    CRUISER,
    SCOOTER,
    TOURING
};

MOTORCYCLE_TYPE convertMotorcycleTypeStrToEnum(std::string);
std::string convertMotorcycleTypeEnumToStr(MOTORCYCLE_TYPE);

class Motorcycle : public Vehicle {
public:
    /**
     * Constructor for Motorcycle
     * @param name name of motorcycle
     * @param price price of motorcycle in dollars
     * @param manufacturer manufacturer of motorcycle
     * @param mileage mileage of motorcycle in kilometres
     * @param horsepower horsepower of motorcycle
     * @param maxSpeed max speed of motorcycle in km/h
     * @param color color of motorcycle
     * @param engineSize size of engine in CC
     * @param maxAcceleration max accel of engine
     * @param motorcycleType type of the motorcycle
     */
    Motorcycle(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed, std::string color, double engineSize, double maxAcceleration, MOTORCYCLE_TYPE motorcycleType);

    /**
     * Get the engine size of the motorcycle
     * @return The engine size of the motorcycle in CC
     */
    double getEngineSize() const;

    /**
     * Get the max acceleration of the motorcycle
     * @return The max acceleration of the motorcycle in m/s^2
     */
    double getMaxAccel() const;

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
     * Deserialize all the data from a JSON file into an instance of Motorcycle.
     * @param data the JSON data to deserialize
     * @return a new Motorcycle instance made using the JSON data.
     * @throws runtime_error if required key does not exist
     */
    static Motorcycle deserializeFromJSON(const json &data);

    /**
     * Loads in a Motorcycle instance from a file given its UUID
     * @param uuid the UUID of the object
     * @return An instance of the deserialized Motorcycle from the JSON file
     * @throws runtime_error if a file with the UUID does not exist
     */
    static Motorcycle loadFromUUID(const std::string& uuid);

    /**
     * Loads in a Motorcycle instance from a file given its path
     * @param path the path of the serialized instance
     * @return An instance of the deserialized Motorcycle from the JSON file
     * @throws runtime_error if a file does not exist at given path
     */
    static Motorcycle loadFromPath(const std::string& path);

    /**
     * Prepares the information within the Motorcycle class to be printed as a string.
     * @return formatted string of all of the info in Motorcycle
     */
    std::string toFormattedString() override;
private:
    double engineSize; // in CC
    double maxAcceleration; // in m/s^2
    MOTORCYCLE_TYPE motorcycleType;
};