#pragma once
#include "Person.hpp"

class Person;

class Vehicle {
    public:
        Vehicle();
        virtual ~Vehicle() = 0;
        bool start();
        bool stop();
        bool drive(double distance);
        bool addDriver(Person* driver);
};