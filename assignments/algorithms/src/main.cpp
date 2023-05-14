/**
 * Name: Sorting
 * Description: write shit here lol
 *
 * @cite Feng Wang, Colorize, (2020), GitHub repository, https://github.com/fengwang/colorize
 * @cite Niels Lohmann, JSON for Modern C++, (2022), https://github.com/nlohmann/json
 *
 * @author Aritro Saha
 * Last edited: May 11, 2023
 */

#define RAND_DEC() ((double) rand() / RAND_MAX)

#ifdef __APPLE__
#define RAND_SEED() sranddev()
#else
#define RAND_SEED() srand(time(nullptr))
#endif

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <filesystem>
#include <future>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <algorithm>
#include "Vehicle.hpp"
#include "colorize.h"
#include "BS_thread_pool.hpp"

using namespace nlohmann;
using namespace std::chrono;
namespace fs = std::filesystem;

const int numToPrint = 40;
const int arrSizes[]{5, 10, 100, 1000, 10000, 30000, 50000, 75000};
const int sampleSize = 200;
const std::string dataDirectory = "data/";

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
    // Set the seed for our random number generator
    RAND_SEED();

    // Thread pool to speed up tasks
    BS::thread_pool thread_pool;

    // Fetch the json data for all car names
    cpr::Response res = cpr::Get(cpr::Url{"https://raw.githubusercontent.com/matthlavacka/car-list/master/car-list.json"});
    assert(res.header["content-type"] == "text/plain; charset=utf-8");
    assert(res.status_code == 200);
    carData = json::parse(res.text);

    // Key function for sorting
    auto vehiclesKeyFunc = [](Vehicle* vehicle){
        return vehicle->getPrice();
    };

    std::map<int, std::vector<Vehicle*>> randomVehiclesSet;
    for (int arrSize : arrSizes) {
        randomVehiclesSet[arrSize] = std::vector<Vehicle*>();
        randomVehiclesSet[arrSize].reserve(arrSize);
        for (int i = 0; i < arrSize; i++) {
            randomVehiclesSet[arrSize].push_back(generateRandomVehicle());
        }
    }

    auto runBenchmarkOnArrSize = [&](int arrSize, int startTestCase=1, int endTestCase=sampleSize) {
        /*
        // Set up CSV file
        if (!fs::is_directory(dataDirectory) || !fs::exists(dataDirectory)) {
            fs::create_directory(dataDirectory);
        }
        std::fstream file;
        file.open(dataDirectory + std::to_string(arrSize) + ".csv", std::ios::out | std::ios::trunc);
        */
        std::stringstream file("");

        /*
        file << "Object Count,"
             << "Test #,"
             << "Unsorted Existing Linear Search,"
             << "Unsorted Absent Linear Search,"
             << "Insertion Sort,"
             << "Built-in Sort,"
             << "Sorted Existing Linear Search,"
             << "Sorted Absent Linear Search,"
             << "Existing Binary Search,"
             << "Absent Binary Search"
             << "\n";
        // std::cout << "Object Count,Test #,Unsorted Existing Linear Search,Unsorted Absent Linear Search,Insertion Sort,Sorted Existing Linear Search,Sorted Absent Linear Search,Existing Binary Search,Absent Binary Search\n";
        */

        // Generate necessary number of vehicles
        /*
        vehicles.reserve(arrSize);
        for (int i = 0; i < arrSize; i++) {
            vehicles.push_back(generateRandomVehicle());
        }
        */

        std::vector<Vehicle*>& vehicles = randomVehiclesSet[arrSize];
       

        // printArray(vehicles);

        for (int testNum = startTestCase; testNum <= endTestCase; testNum++) {
            // Cloned so we don't have to generate vehicles for each sample
            std::vector<Vehicle*> sortedVehicles{vehicles};
            std::vector<Vehicle*> builtInSortedVehicles{vehicles};

            // Run a linear search for an existing object
            double valToLookFor = vehiclesKeyFunc(sortedVehicles[rand() % vehicles.size()]);
            auto start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(vehicles, valToLookFor, vehiclesKeyFunc);
            auto stop = high_resolution_clock::now();
            auto existingLinearSearchBeforeSortDuration = duration_cast<nanoseconds>(stop - start).count();

            // Run a linear search for an object that doesn't exist
            start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(vehicles, 1.0e10, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto nonExistingLinearSearchBeforeSortDuration = duration_cast<nanoseconds>(stop - start).count();

            // Sort the entire array using insertion sort
            start = high_resolution_clock::now();
            insertionSort<Vehicle*, double>(sortedVehicles, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto insertionSortDuration = duration_cast<nanoseconds>(stop - start).count();

            // Sort the entire array using func from STD
            start = high_resolution_clock::now();
            std::sort(builtInSortedVehicles.begin(), builtInSortedVehicles.end(), [vehiclesKeyFunc](Vehicle* a, Vehicle* b){
                return vehiclesKeyFunc(a) < vehiclesKeyFunc(b);
            });
            stop = high_resolution_clock::now();
            auto builtInSortDuration = duration_cast<nanoseconds>(stop - start).count();

            // Run a linear search on the sorted array for an existing object
            valToLookFor = vehiclesKeyFunc(sortedVehicles[rand() % vehicles.size()]);
            start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(sortedVehicles, valToLookFor, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto existingLinearSearchAfterSortDuration = duration_cast<nanoseconds>(stop - start).count();

            // Run a linear search for an object that doesn't exist
            start = high_resolution_clock::now();
            linearSearch<Vehicle*, double>(sortedVehicles, 1.0e10, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto nonExistingLinearSearchAfterSortDuration = duration_cast<nanoseconds>(stop - start).count();

            // Run a binary search on the sorted array for an existing object
            valToLookFor = vehiclesKeyFunc(sortedVehicles[rand() % vehicles.size()]);
            start = high_resolution_clock::now();
            binarySearch<Vehicle*, double>(sortedVehicles, valToLookFor, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto existingBinarySearchAfterSortDuration = duration_cast<nanoseconds>(stop - start).count();

            // Run a binary search for an object that doesn't exist
            start = high_resolution_clock::now();
            binarySearch<Vehicle*, double>(sortedVehicles, 1.0e10, vehiclesKeyFunc);
            stop = high_resolution_clock::now();
            auto nonExistingBinarySearchAfterSortDuration = duration_cast<nanoseconds>(stop - start).count();

            file << arrSize << ","
                 << testNum << ","
                 << existingLinearSearchBeforeSortDuration << ","
                 << nonExistingLinearSearchBeforeSortDuration << ","
                 << insertionSortDuration << ","
                 << builtInSortDuration << ","
                 << existingLinearSearchAfterSortDuration << ","
                 << nonExistingLinearSearchAfterSortDuration << ","
                 << existingBinarySearchAfterSortDuration << ","
                 << nonExistingBinarySearchAfterSortDuration << "\n";

            /*
            std::cout << arrSize << ","
                      << testNum << ","
                      << existingLinearSearchBeforeSortDuration << ","
                      << nonExistingLinearSearchBeforeSortDuration << ","
                      << insertionSortDuration << ","
                      << builtInSortDuration << ","
                      << existingLinearSearchAfterSortDuration << ","
                      << nonExistingLinearSearchAfterSortDuration << ","
                      << existingBinarySearchAfterSortDuration << ","
                      << nonExistingBinarySearchAfterSortDuration << "\n";
            */
        }

        // Clear out the vehicles vec
        // for (Vehicle* vehicle : vehicles) {
        //     delete vehicle;
        // }
        // vehicles.clear();
        return file.str();

        // file.close();
    };

    auto start = high_resolution_clock::now();

    std::vector<std::future<std::string>> futures;

    for (const int arrSize : arrSizes) {
       for (int testNum = 1; testNum <= sampleSize; testNum++) {
            futures.push_back(thread_pool.submit(runBenchmarkOnArrSize, arrSize, testNum, testNum));
       }
    }

    std::vector<std::string> results;
    for (auto& future : futures) {
        future.wait();
        std::string tmp(future.get());
        results.push_back(tmp);
        std::cout << tmp;
    }


    auto stop = high_resolution_clock::now();
    auto totalDuration = duration_cast<seconds>(stop - start).count();

    for (auto str : results) {
        std::cout << str;
        // todo: add to file
    }

    std::cout << "Complete, took " << totalDuration << "s.\n";

    return 0;
}
