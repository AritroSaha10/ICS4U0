#pragma once

#include <vector>
#include <string>

/**
 * A class representing a Vehicle. Serves as a base class for specific types of vehicles (sedans, motorcycles, etc.).
 */
class Vehicle {
public:
    /**
     * Constructor for Vehicle
     * @param name name of Vehicle
     * @param price price of Vehicle in dollars
     * @param wheels wheels on Vehicle
     * @param doors doors on Vehicle
     * @param seats seats in Vehicle
     * @param mileage mileage of Vehicle in kilometres
     * @param horsepower horsepower of Vehicle
     * @param maxSpeed max speed of Vehicle in km/h
     */
    Vehicle(std::string name, double price, int wheels, int doors, int seats,
            double mileage, double horsepower, double maxSpeed);

    /**
     * Change the price of the vehicle
     * @param newPrice the new price
     * @return Whether the price was successfully changed
     */
    bool changePrice(double newPrice);

    /**
     * Get the max speed of the vehicle in km/h
     * @return max speed of vehicle in km/h
     */
    double getMaxSpeed() const;

    /**
     * Get the number of wheels that the vehicle has
     * @return # of wheels that vehicle has
     */
    int getWheels() const;

    /**
     * Get the number of doors that the vehicle has
     * @return # of doors that vehicle has
     */
    int getDoors() const;

    /**
     * Get the number of seats that the vehicle has
     * @return # of seats that vehicle has
     */
    int getSeats() const;

    /**
     * Get the horsepower available from the vehicle's engine
     * @return the amount of horsepower of the vehicle
     */
    double getHorsepower() const;

    /**
     * Get the price of the vehicle in dollars
     * @return price of the vehicle in dollars
     */
    double getPrice() const;

    /**
     * Get the name of the vehicle
     * @return name of the vehicle
     */
    std::string getName() const;

    /**
     * Convert the data in the instance into a string
     * @param out the current output stream to add onto
     * @param obj reference to Vehicle to get data from
     * @return the manipulated output stream with the Vehicle data
     */
    friend std::ostream &operator<<(std::ostream &out, const Vehicle &obj);

protected:
    std::string name;
    double price;
    int wheels;
    int doors;
    int seats;
    double maxSpeed;
    double mileage;
    double horsepower;
};