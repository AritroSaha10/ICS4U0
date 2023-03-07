#pragma once
#include <vector>
#include "Vehicle.hpp"
#include "MMI_Features.hpp"

class Sedan : public Vehicle {
public:
    Sedan(std::string name, double price, std::string manufacturer, double mileage, double horsepower, std::string color);
    double getMaxCapacity();
    double getTowingMaxLoad();
    std::string serializeToJSON();
    ~Sedan();
private:
    double trunkCapacity;
    double towingMaxLoad;
    std::vector<MMI_Features> mmiFeatures;
};