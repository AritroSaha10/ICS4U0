/**
 * Name: Sorting
 * Description: write shit here lol
 *
 * @cite Feng Wang, Colorize, (2020), GitHub repository, https://github.com/fengwang/colorize
 * @cite Niels Lohmann, JSON for Modern C++, (2022), https://github.com/nlohmann/json
 *
 * @author Aritro Saha
 * Last edited: May 10, 2023
 */

#define RAND_DEC() ((double) rand() / RAND_MAX)

#include <iostream>
#include <vector>
#include <chrono>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "Vehicle.hpp"
#include "colorize.h"

using namespace nlohmann;
using namespace std::chrono;

const int numToPrint = 10;
const int arrSizes[]{75000};
const int sampleSize = 10;
const std::string dataPath = "data.csv";

std::vector<Vehicle*> vehicles;
json carData;

double roundTo(double value, double precision = 1.0) {
    return std::round(value / precision) * precision;
}

/**
 * Print the first and last 20 values of pointers in order with nice formatting
 * @tparam T the type used in the vector
 * @param vec vector to get pointers from
 */
template<class T>
void printArray(std::vector<T *> &vec) {
    assert(numToPrint % 2 == 0);

    int halfwayPoint = numToPrint / 2;

    if (vec.size() > numToPrint) {
        for (int i = 0; i < halfwayPoint; i++) {
            std::cout << i + 1 << ". " << *vec[i] << "\n";
        }

        std::cout << "[... " << vec.size() - (numToPrint + 1) << " more elements ...]\n";

        for (int i = vec.size() - (halfwayPoint + 1); i < vec.size(); i++) {
            std::cout << i + 1 << ". " << *vec[i] << "\n";
        }
    } else {
        for (int i = 0; i < vec.size(); i++) {
            std::cout << i + 1 << ". " << *vec[i] << "\n";
        }
    }
}

template<class T1, class T2>
int linearSearch(std::vector<T1>& vec, T2 value, std::function<T2(T1)> extractKey) {
    for (int i = 0; i < vec.size(); i++) {
        if (extractKey(vec[i]) == value) {
            return i;
        }
    }

    return -1;
}

template<class T1, class T2>
int binarySearch(std::vector<T1>& vec, T2 value, std::function<T2(T1)> extractKey) {
    int start = 0, end = vec.size() - 1;
    while (start <= end) {
        int middle = (start + end) / 2;
        T2 key = extractKey(vec[middle]);

        if (key == value) {
            return middle;
        } else if (key < value) {
            start = middle + 1;
        } else {
            end = middle - 1;
        }
    }

    return -1;
}

template<class T1, class T2>
std::vector<T1> insertionSort(std::vector<T1>& vec, std::function<T2(T1)> extractKey) {
    for (int i = 0; i <= vec.size() - 1; i++) {
        Vehicle* element = vec[i];
        T2 key = extractKey(element); // use current element as point of comparison
        int j = i - 1; // set idx to start comparison with

        // While the element before it is larger, swap them
        for (; j >= 0 && extractKey(vec[j]) > key; j--) {
            vec[j + 1] = vec[j];
        }

        // Swap original comparison element to the last swapped element
        vec[j + 1] = element;
    }

    return vec;
}

Vehicle* generateRandomVehicle() {
    std::string name;
    double price, mileage, horsepower, maxSpeed;
    int wheels, doors, seats;

    price = roundTo(20000.0 + RAND_DEC() * 100000.0, 0.01);
    mileage = roundTo(RAND_DEC() * 100000.0, 0.01);
    horsepower = roundTo( RAND_DEC() * 200.0, 0.01);
    maxSpeed = roundTo(70.0 + RAND_DEC() * 300.0, 0.01);
    wheels = 2 + rand() % 10;
    doors = 2 + rand() % 10;
    seats = 2 + rand() % 10;

    json manufacturerInfo = carData[rand() % carData.size()];
    std::string manufacturer = manufacturerInfo["brand"];
    json models = manufacturerInfo["models"];
    std::string model = models[rand() % models.size()];
    std::string year = std::to_string(1990 + rand() % 33);

    name = manufacturer + " " + model + " " + year;

    return new Vehicle(name, price, wheels, doors, seats, mileage, horsepower, maxSpeed);
}

int main() {
    // Set up CSV file
    std::fstream file;
    file.open(dataPath, std::ios::out | std::ios::trunc);
    file << "Object Count,Test #,Unsorted Existing Linear Search,Unsorted Absent Linear Search,Insertion Sort,Sorted Existing Linear Search,Sorted Absent Linear Search,Existing Binary Search,Absent Binary Search\n";
    std::cout << "Object Count,Test #,Unsorted Existing Linear Search,Unsorted Absent Linear Search,Insertion Sort,Sorted Existing Linear Search,Sorted Absent Linear Search,Existing Binary Search,Absent Binary Search\n";

    // Set the seed for our random number generator
    sranddev();

    // Fetch the json data for all car names
    cpr::Response res = cpr::Get(cpr::Url{"https://raw.githubusercontent.com/matthlavacka/car-list/master/car-list.json"});
    assert(res.header["content-type"] == "text/plain; charset=utf-8");
    assert(res.status_code == 200);
    carData = json::parse(res.text);

    // Key function for sorting
    auto vehiclesKeyFunc = [](Vehicle* vehicle){
        return vehicle->getPrice();
    };

    for (const int arrSize : arrSizes) {
        // Clear out the vehicles vec
        for (Vehicle* vehicle : vehicles) {
            delete vehicle;
        }
        vehicles.clear();

        // Generate necessary number of vehicles
        for (int i = 0; i < arrSize; i++) {
            vehicles.push_back(generateRandomVehicle());
        }

        printArray(vehicles);

        for (int testNum = 1; testNum <= sampleSize; testNum++) {
            // Cloned so we don't have to generate vehicles for each sample
            std::vector<Vehicle*> sortedVehicles{vehicles};

            // Run a linear search for an existing object
            auto start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(vehicles, vehiclesKeyFunc(vehicles[rand() % vehicles.size()]), vehiclesKeyFunc);
            auto stop = high_resolution_clock::now();
            auto existingLinearSearchBeforeSortDuration = duration_cast<microseconds>(stop - start).count();

            // Run a linear search for an object that doesn't exist
            start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(vehicles, -1.0, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto nonExistingLinearSearchBeforeSortDuration = duration_cast<microseconds>(stop - start).count();

            // Sort the entire array
            start = high_resolution_clock::now();
            insertionSort<Vehicle*, double>(sortedVehicles, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto insertionSortDuration = duration_cast<microseconds>(stop - start).count();

            // Run a linear search on the sorted array for an existing object
            start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(sortedVehicles, vehiclesKeyFunc(sortedVehicles[rand() % vehicles.size()]), vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto existingLinearSearchAfterSortDuration = duration_cast<microseconds>(stop - start).count();

            // Run a linear search for an object that doesn't exist
            start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(sortedVehicles, -1.0, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto nonExistingLinearSearchAfterSortDuration = duration_cast<microseconds>(stop - start).count();

            // Run a binary search on the sorted array for an existing object
            start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(sortedVehicles, vehiclesKeyFunc(sortedVehicles[rand() % vehicles.size()]), vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto existingBinarySearchAfterSortDuration = duration_cast<microseconds>(stop - start).count();

            // Run a binary search for an object that doesn't exist
            start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(sortedVehicles, -1.0, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto nonExistingBinarySearchAfterSortDuration = duration_cast<microseconds>(stop - start).count();

            file << arrSize << ","
                << testNum << ","
                << existingLinearSearchBeforeSortDuration << ","
                << nonExistingLinearSearchBeforeSortDuration << ","
                << insertionSortDuration << ","
                << existingLinearSearchAfterSortDuration << ","
                << nonExistingLinearSearchAfterSortDuration << ","
                << existingBinarySearchAfterSortDuration << ","
                << nonExistingBinarySearchAfterSortDuration << "\n";

            std::cout << arrSize << ","
                      << testNum << ","
                      << existingLinearSearchBeforeSortDuration << ","
                      << nonExistingLinearSearchBeforeSortDuration << ","
                      << insertionSortDuration << ","
                      << existingLinearSearchAfterSortDuration << ","
                      << nonExistingLinearSearchAfterSortDuration << ","
                      << existingBinarySearchAfterSortDuration << ","
                      << nonExistingBinarySearchAfterSortDuration << "\n";
        }
    }

    file.close();
    return 0;
}