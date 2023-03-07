#pragma once
#include <vector>
#include "Person.hpp"

class Person;

class Vehicle {
public:
    Vehicle(std::string name, double price, int wheels, int doors, int seats, int maxPassengers, std::string manufacturer, double mileage, double horsepower, std::string color);
    virtual ~Vehicle() = 0;
    bool start();
    bool stop();
    bool drive(double distance);
    bool addDriver(Person* driver);
    bool removeDriver();
    bool addPassenger(Person* passenger);
    bool removePassenger(int idx);
    bool changeColor(std::string newColor);
    virtual std::string serializeToJSON() = 0;
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
};