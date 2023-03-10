#pragma once
#include "Vehicle.hpp"

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
     */
    Motorcycle(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed, std::string color);

    /**
     * Serialize the data to a JSON file
     * @return JSON version of the instance
     */
    json serializeToJSON() override;
};