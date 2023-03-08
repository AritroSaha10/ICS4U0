#pragma once
#include <vector>
#include "Vehicle.hpp"
#include "MMI_Features.hpp"

class Sedan : public Vehicle {
public:
    Sedan(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed, double trunkCapacity, double towingMaxLoad, std::string color);
    double getTrunkCapacity() const;
    double getTowingMaxLoad() const;
    json serializeToJSON() override;
    static Sedan deserializeFromJSON(const json &data);
    void saveAsFile();
    static Sedan loadFromUUID(std::string uuid);
private:
    double trunkCapacity;
    double towingMaxLoad;
    std::vector<MMI_Features> mmiFeatures;
};