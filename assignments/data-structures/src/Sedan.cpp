#include "Sedan.hpp"

Sedan::Sedan(std::string name, double price, std::string manufacturer, double mileage, double horsepower, double maxSpeed,
             double trunkCapacity, double towingMaxLoad, std::string color) : Vehicle(name, price, 4, 4, 5, 4, manufacturer, mileage, horsepower, maxSpeed, color) {
    this->trunkCapacity = trunkCapacity;
    this->towingMaxLoad = towingMaxLoad;
}

double Sedan::getTrunkCapacity() const {
    return trunkCapacity;
}

double Sedan::getTowingMaxLoad() const {
    return towingMaxLoad;
}

json Sedan::serializeToJSON() {
    json vehicleSerialized = Vehicle::serializeToJSON();
    vehicleSerialized["trunkCapacity"] = trunkCapacity;
    vehicleSerialized["towingMaxLoad"] = towingMaxLoad;

    return vehicleSerialized;
}
