/**
 * Name: Algorithm Benchmarker (Multithreaded)
 * Description: A benchmarking programming that runs various different sorting &
 * searching algorithms on arrays of different sizes. Its settings can also be
 * dynamically configured. For example, you can change the different array sizes,
 * sample sizes, and more just from the top of this file. In addition, it uses
 * multithreading in order to speed up the benchmarking processes and utilize
 * the entire CPU.
 *
 * @cite Feng Wang, Colorize, (2020), GitHub repository, https://github.com/fengwang/colorize
 * @cite Niels Lohmann, JSON for Modern C++, (2022), https://github.com/nlohmann/json
 * @cite Barak Shoshany, BS::thread_pool (2023), GitHub repository, https://github.com/bshoshany/thread-pool.git
 * @cite GeeksForGeeks, Binary Search – Data Structure and Algorithm Tutorials, Article, https://www.geeksforgeeks.org/binary-search/
 * @cite GeeksForGeeks, Insertion Sort – Data Structure and Algorithm Tutorials, Article, https://www.geeksforgeeks.org/insertion-sort/
 * @cite GeeksForGeeks, IntroSort or Introspective Sort, Article, https://www.geeksforgeeks.org/introsort-or-introspective-sort/
 *
 * @author Aritro Saha
 * Last edited: May 17, 2023
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
const std::string dataPath = "data.csv";

json carData;

/**
 * Round a value to a specific precision
 * @param value Value to round
 * @param precision Precision to round to (ex. round to nearest 100)
 * @return Rounded value
 */
double roundTo(double value, double precision = 1.0) {
    return std::round(value / precision) * precision;
}

/**
 * Gets the required key from a vehicle.
 * @param vehicle Vehicle object to get key from
 * @return key value
 */
double getKeyFromVehicle(Vehicle* vehicle) {
    return vehicle->getPrice();
}

/**
 * Compares two vehicle objects
 * @param a first vehicle
 * @param b second vehicle
 * @return if a < b
 */
bool compareVehicles(Vehicle* a, Vehicle* b) {
    return getKeyFromVehicle(a) < getKeyFromVehicle(b);
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

/**
 * Linearly search for a value. O(n)
 * @tparam T1 Vector element type
 * @tparam T2 Element key type
 * @param vec Vector to search
 * @param value Value to search for
 * @param extractKey Function to extract key value from
 * @return Index of item
 */
template<class T1, class T2>
int linearSearch(std::vector<T1>& vec, T2 value, std::function<T2(T1)> extractKey) {
    for (int i = 0; i < vec.size(); i++) {
        if (extractKey(vec[i]) == value) {
            return i;
        }
    }

    return -1;
}

/**
 * Search for a value using binary search. O(log n)
 * @tparam T1 Vector element type
 * @tparam T2 Element key type
 * @param vec Sorted vector to search
 * @param value Value to search for
 * @param extractKey Function to extract key value from
 * @return Index of item
 */
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

/**
 * Runs insertion sort on a vector array. Changes the vector in place.
 * @tparam T1 Data type for unsorted vector
 * @param vec Vector to sort
 * @param compareFunc Function to compare one element to another. Should return true if a < b
 * @return A sorted version of the array.
 */
template<class T1>
std::vector<T1> insertionSort(std::vector<T1>& vec, std::function<bool(T1 a, T1 b)> compareFunc) {
    for (int i = 0; i <= vec.size() - 1; i++) {
        Vehicle* element = vec[i];
        int j = i - 1; // set idx to start comparison with

        // While the element before it is larger, swap them
        for (; j >= 0 && !compareFunc(vec[j], element); j--) {
            vec[j + 1] = vec[j];
        }

        // Swap original comparison element to the last swapped element
        vec[j + 1] = element;
    }

    return vec;
}

/**
 * Generates a random Vehicle with random data.
 * @return Pointer to new Vehicle instance
 */
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

    // Store all the random sets of Vehicles first, don't have to re-gen per thread
    std::map<int, std::vector<Vehicle*>> randomVehiclesSet;

    // Only generate a random set of Vehicles for the largest array size
    const int largestArrSize = arrSizes[(sizeof(arrSizes) / sizeof(arrSizes[0])) - 1];
    randomVehiclesSet[largestArrSize] = std::vector<Vehicle*>();
    randomVehiclesSet[largestArrSize].reserve(largestArrSize);
    for (int i = 0; i < largestArrSize; i++) {
        randomVehiclesSet[largestArrSize].push_back(generateRandomVehicle());
    }

    // Fill the rest of the arrays by getting subsets of the largest array
    for (int i = 0; i < (sizeof(arrSizes) / sizeof(arrSizes[0])) - 1; i++) {
        const int arrSize = arrSizes[i];
        randomVehiclesSet[arrSize] = std::vector<Vehicle*>(
                randomVehiclesSet[largestArrSize].begin(),
                randomVehiclesSet[largestArrSize].begin() + arrSize
        );
    }

    auto runBenchmarkOnArrSize = [&](int arrSize, int testNum) {
        std::stringstream ss("");

        // Generate necessary number of vehicles
        std::vector<Vehicle*>& vehicles = randomVehiclesSet[arrSize];

        // Cloned so we don't have to generate vehicles for each sample
        std::vector<Vehicle*> sortedVehicles{vehicles};
        std::vector<Vehicle*> builtInSortedVehicles{vehicles};

        // Run a linear search for an existing object
        double valToLookFor = sortedVehicles[rand() % vehicles.size()]->getPrice();
        auto start = high_resolution_clock::now();
        linearSearch<Vehicle*, double>(vehicles, valToLookFor, getKeyFromVehicle);
        auto stop = high_resolution_clock::now();
        auto existingLinearSearchBeforeSortDuration = duration_cast<nanoseconds>(stop - start).count();

        // Run a linear search for an object that doesn't exist
        start = high_resolution_clock::now();
        linearSearch<Vehicle*, double>(vehicles, 1.0e10, getKeyFromVehicle);
        stop = high_resolution_clock::now();
        auto nonExistingLinearSearchBeforeSortDuration = duration_cast<nanoseconds>(stop - start).count();

        // Sort the entire array using insertion sort
        start = high_resolution_clock::now();
        insertionSort<Vehicle*>(sortedVehicles, compareVehicles);
        stop = high_resolution_clock::now();
        auto insertionSortDuration = duration_cast<nanoseconds>(stop - start).count();

        // Sort the entire array using func from STD
        start = high_resolution_clock::now();
        std::sort(builtInSortedVehicles.begin(), builtInSortedVehicles.end(), compareVehicles);
        stop = high_resolution_clock::now();
        auto builtInSortDuration = duration_cast<nanoseconds>(stop - start).count();

        // Run a linear search on the sorted array for an existing object
        start = high_resolution_clock::now();
        linearSearch<Vehicle*, double>(builtInSortedVehicles, valToLookFor, getKeyFromVehicle);
        stop = high_resolution_clock::now();
        auto existingLinearSearchAfterSortDuration = duration_cast<nanoseconds>(stop - start).count();

        // Run a linear search for an object that doesn't exist
        start = high_resolution_clock::now();
        linearSearch<Vehicle*, double>(builtInSortedVehicles, 1.0e10, getKeyFromVehicle);
        stop = high_resolution_clock::now();
        auto nonExistingLinearSearchAfterSortDuration = duration_cast<nanoseconds>(stop - start).count();

        // Run a binary search on the sorted array for an existing object
        start = high_resolution_clock::now();
        binarySearch<Vehicle*, double>(builtInSortedVehicles, valToLookFor, getKeyFromVehicle);
        stop = high_resolution_clock::now();
        auto existingBinarySearchAfterSortDuration = duration_cast<nanoseconds>(stop - start).count();

        // Run a binary search for an object that doesn't exist
        start = high_resolution_clock::now();
        binarySearch<Vehicle*, double>(builtInSortedVehicles, 1.0e10, getKeyFromVehicle);
        stop = high_resolution_clock::now();
        auto nonExistingBinarySearchAfterSortDuration = duration_cast<nanoseconds>(stop - start).count();
        
        // Push all our CSV data into the stream
        ss << arrSize << ","
           << testNum << ","
           << existingLinearSearchBeforeSortDuration << ","
           << nonExistingLinearSearchBeforeSortDuration << ","
           << insertionSortDuration << ","
           << builtInSortDuration << ","
           << existingLinearSearchAfterSortDuration << ","
           << nonExistingLinearSearchAfterSortDuration << ","
           << existingBinarySearchAfterSortDuration << ","
           << nonExistingBinarySearchAfterSortDuration << "\n";

        // Turn it into a string from a stream before returning
        return ss.str();
    };
    
    // Vector to store all futures
    std::vector<std::future<std::string>> futures;
    
    // Start the timer
    auto start = high_resolution_clock::now();

    // Generate all the tasks for the thread pool
    for (const int arrSize : arrSizes) {
        for (int testNum = 1; testNum <= sampleSize; testNum++) {
            futures.push_back(thread_pool.submit(runBenchmarkOnArrSize, arrSize, testNum));
        }
    }

    // Wait for all of their results
    std::vector<std::string> results;
    for (auto& future : futures) {
        future.wait();
        std::string tmp(future.get());
        results.push_back(tmp);
        // std::cout << tmp; // Removed to improve performance
    }

    // Get stop time and calculate total duration
    auto stop = high_resolution_clock::now();
    auto totalDuration = duration_cast<seconds>(stop - start).count();

    // Set up CSV file
    std::fstream file;
    file.open(dataPath, std::ios::out | std::ios::trunc);

    // Setup file header
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

    // Push all futures into file
    for (const auto& str : results) {
        std::cout << str;
        file << str;
    }

    // Close file
    file.close();

    std::cout << "Complete, took " << totalDuration << "s.\n";

    return 0;
}
