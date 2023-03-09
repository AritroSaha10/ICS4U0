#pragma once
#include <vector>
#include "Person.hpp"

class Person;

class Vehicle {
public:
    Vehicle(std::string name, double price, int wheels, int doors, int seats, int maxPassengers, std::string manufacturer, double mileage, double horsepower, double maxSpeed, std::string color);
    bool start();
    bool stop();
    bool drive(double distance);
    bool addDriver(Person* driver);
    bool removeDriver();
    bool addPassenger(Person* passenger);
    bool removePassenger(Person* passenger);
    bool removePassenger(int idx);
    bool changeColor(const std::string& newColor);
    bool changePrice(double newPrice);
    double getMaxSpeed() const;
    double getWheels() const;
    double getDoors() const;
    double getSeats() const;
    double getHorsepower() const;
    double getPrice() const;
    std::string getName() const;
    virtual json serializeToJSON();
    static Vehicle deserializeFromJSON(const json &data);
    void saveAsFile();
    static Vehicle loadFromUUID(std::string uuid);

    friend std::ostream & operator <<(std::ostream &out, const Vehicle &obj);
protected:
    std::string name;
    double price;
    int wheels;
    int doors;
    int seats;
    int maxPassengers;
    double maxSpeed;
    std::string manufacturer;
    double mileage;
    double horsepower;
    Person* driver;
    std::vector<Person*> passengers;
    bool started;
    std::string color;
    std::string uuid;
};