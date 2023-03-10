// !!!!! READ BELOW
// Ask Ms. Willis if we are allowed to keep classes / methods that we implemented and designed
// but didn't get the chance to implement in our main code

#include <iostream>
#include "BankAccount.hpp"
#include "Person.hpp"
#include "VehicleDealership.hpp"
#include "util.hpp"
#include "colorize.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
using namespace std::chrono;

Person* playerData;
std::vector<Person*> people;
std::vector<VehicleDealership*> dealerships;

/*
void testSerializationBankAccount() {
    BankAccount bankAccount(100, 0, 2500, 2500);
    std::cout << "Saving this bank account: " << bankAccount.serializeToJSON() << "\n";
    std::string uuid = bankAccount.getUUID();
    bankAccount.saveAsFile();

    BankAccount newBankAccount = BankAccount::loadFromUUID(uuid);
    std::cout << "Loaded JSON: " << bankAccount.serializeToJSON() << "\n";
}
 */

/**
 * Print a list of items in order with nice formatting
 * @tparam T the type used in the vector
 * @param vec vector to get items from
 */
template <class T>
void printListWithIdx(std::vector<T> &vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << i + 1 << ". " << vec[i] << "\n";
    }
}

/**
 * Print a list of values of pointers in order with nice formatting
 * @tparam T the type used in the vector
 * @param vec vector to get pointers from
 */
template <class T>
void printPointerValsWithIdx(std::vector<T*> &vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << i + 1 << ". " << *vec[i] << "\n";
    }
}

/**
 * Generate a Person pointer from user input.
 * @return the generated Person
 */
Person* generatePersonFromInput() {
    std::string firstName, middleName, lastName;
    int birthYear, birthMonth, birthDay;
    double height;
    // Bank account details
    double startingBalance, minBalance, withdrawLimit, depositLimit;

    std::cout << color::rize("-- Create a person --\n", "White", "Green");
    std::cout << "First name: ";
    std::cin >> firstName;
    std::cout << "Middle name: ";
    std::getline(std::cin, middleName);
    std::getline(std::cin, middleName);
    std::cout << "Last name: ";
    std::cin >> lastName;

    std::cout << "Birth year: ";
    std::cin >> birthYear;
    std::cout << "Birth month: ";
    std::cin >> birthMonth;
    std::cout << "Birth day: ";
    std::cin >> birthDay;

    std::cout << "Height (cm): ";
    std::cin >> height;

    std::cout << color::rize("- Banking details -\n", "White", "Blue");
    std::cout << "Starting Balance: ";
    std::cin >> startingBalance;
    std::cout << "Minimum balance: ";
    std::cin >> minBalance;
    std::cout << "Withdraw limit: ";
    std::cin >> withdrawLimit;
    std::cout << "Deposit limit: ";
    std::cin >> depositLimit;

    // Convert birth date to timestamp
    struct tm myTm{};
    time_t rawTime;
    time(&rawTime);
    myTm = *localtime(&rawTime);
    myTm.tm_year = birthYear - 1900;
    myTm.tm_mon = birthMonth - 1;
    myTm.tm_mday = birthDay;
    int64_t birthTimestamp = mktime(&myTm);

    return new Person{firstName, middleName, lastName, birthTimestamp, height,
            new BankAccount( startingBalance, minBalance, withdrawLimit, depositLimit)};
}

/**
 * Generate a VehicleDealership pointer from user input.
 * @return the generated VehicleDealership
 */
VehicleDealership* generateDealershipFromInput() {
    std::cout << color::rize("-- Create a vehicle dealership --\n", "White", "Green");
    std::string name;
    // Bank account details
    double startingBalance, minBalance, withdrawLimit, depositLimit;

    std::cout << "Name of vehicle dealership: ";
    std::getline(std::cin, name); // Required to make sure that it captures input with spaces properly
    std::getline(std::cin, name);

    std::cout << color::rize("- Banking details -\n", "White", "Blue");
    std::cout << "Starting Balance: ";
    std::cin >> startingBalance;
    std::cout << "Minimum balance: ";
    std::cin >> minBalance;
    std::cout << "Withdraw limit: ";
    std::cin >> withdrawLimit;
    std::cout << "Deposit limit: ";
    std::cin >> depositLimit;

    return new VehicleDealership{name, new BankAccount(startingBalance, minBalance, withdrawLimit, depositLimit)};
}

/**
 * Generate a Vehicle pointer and give it to a dealership through user input
 * @return Pointer to the generated Vehicle
 */
Vehicle* generateVehicleFromUserInputAndAttach() {
    if (dealerships.empty()) {
        std::cout << "ERROR: You cannot generate a vehicle without any vehicle dealerships. Please make one before making a vehicle.";
        return nullptr;
    }

    // Header for this section
    std::cout << color::rize("-- Create a vehicle --\n", "White", "Green");

    // Get all necessary info
    auto name = promptFullLineWithValidation("Complete name of vehicle (ex. Honda Accord 2023 Touring Hybrid): ", [](const std::string& str){ return !str.empty(); });
    auto price = promptWithValidation<double>("Price of vehicle: $", [](double x){ return x >= 0; });
    auto manufacturer = promptFullLineWithValidation("Manufacturer: ", [](const std::string& str){ return !str.empty(); });
    auto wheels = promptWithValidation<int>("# of wheels on vehicle: ", [](int x){ return x > 0; });
    auto doors = promptWithValidation<int>("# of doors on vehicle: ", [](int x){ return x > 0; });
    auto seats = promptWithValidation<int>("# of seats in vehicle: ", [](int x){ return x > 0; });
    auto maxPassengers = promptWithValidation<int>("Maximum # of passengers in vehicle: ", [](int x){ return x >= 0; });
    auto mileage = promptWithValidation<double>("Starting mileage on vehicle (km): ", [](double x){ return x >= 0; });
    auto horsepower = promptWithValidation<double>("Horsepower available: ", [](double x){ return x >= 0; });
    auto maxSpeed = promptWithValidation<double>("Max speed available (km/h): ", [](double x){ return x >= 0; });
    auto color = promptFullLineWithValidation("Color (ex. Canyon River Blue Metallic): ", [](const std::string& str){ return !str.empty(); });

    // Create a vehicle from info
    auto vehicle = new Vehicle(name, price, wheels, doors, seats, maxPassengers, manufacturer, mileage, horsepower, maxSpeed, color);

    // Assign the new vehicle to a dealership
    std::cout << "Vehicle successfully created: " << *vehicle << "\n";
    std::cout << "Please choose a dealership to add it under.\n";
    for (int i = 0; i < dealerships.size(); i++) {
        std::cout << i + 1 << ". " << dealerships[i]->getName() << "\n";
    }

    auto idx = promptWithValidation<int>("Enter the index of the dealership to add the vehicle to: ", [](int x){ return x > 0 && x <= dealerships.size(); }) - 1;

    dealerships[idx]->giveVehicle(vehicle);
    std::cout << "Successfully generated vehicle for " << dealerships[idx]->getName() << "!\n";

    return vehicle;
}

/**
 * Create all of the necessary data directories if they don't exist yet.
 */
void createDataDirs() {
    // Make data directory if needed
    if (!fs::is_directory("data") || !fs::exists("data")) { // Check if folder exists
        fs::create_directory("data");
    }

    // Make people directory if needed
    if (!fs::is_directory("data/people") || !fs::exists("data/people")) {
        fs::create_directory("data/people");
    }

    // Make dealership directory if needed
    if (!fs::is_directory("data/vehicle-dealership") || !fs::exists("data/vehicle-dealership")) {
        fs::create_directory("data/vehicle-dealership");
    }
}

/**
 * Using user input, switch the current pointer held in playerData, given the choice of all players.
 */
void switchCurrentPlayerAccount() {
    std::cout << color::rize("List of preloaded people:\n", "Green");
    printPointerValsWithIdx<Person>(people);

    std::string ans;
    do {
        std::cout << "Would you like to import your account if any exist (i), or create a new one (c)? ";
        std::cin >> ans;
    } while (std::cin.fail() || !((ans == "i" && !people.empty()) || ans == "c"));

    if (ans == "c") {
        people.push_back(generatePersonFromInput());
        playerData = people[people.size() - 1];
    } else if (ans == "i") {
        int idx;
        do {
            std::cout << "\n" << "Enter the index of the account to import: ";
            std::cin >> idx;
            idx--;
        } while (std::cin.fail() || idx >= people.size() || idx < 0);
        playerData = people[idx];
    }

    std::cout << "Your info: " << *playerData << "\n";
}

/**
 * Serialized and save all the Person and VehicleDealership data in data/
 */
void saveAllData() {
    for (auto person : people) {
        person->saveAsFile();
    }

    for (auto dealership : dealerships) {
        dealership->saveAsFile();
    }

    std::cout << "Saved all data!\n";
}

int main() {
    std::cout << color::rize("Welcome to the Car Simulator!\n", "Red", "Default", "Bold");

    // Load in all people
    createDataDirs();
    std::string peopleDataPath = "data/people";
    for (const auto & entry : fs::directory_iterator(peopleDataPath)) {
        people.push_back(new Person(Person::loadFromPath(entry.path().string())));
    }
    // Load in all dealerships
    std::string dealershipsDataPath = "data/vehicle-dealership";
    for (const auto & entry : fs::directory_iterator(dealershipsDataPath))
        dealerships.push_back(new VehicleDealership(VehicleDealership::loadFromPath(entry.path().string())));

    switchCurrentPlayerAccount();
    std::cout << "\n\n";

    while (true) {
        int choice = -1;

        std::cout << "What would you like to do?\n";
        std::cout << "-- User actions --\n";
        std::cout << "  1: Switch current user account\n";
        std::cout << "  2: View your own info\n";
        std::cout << "  3: View info about your bank account\n";
        std::cout << "  4: View your own vehicles\n";
        std::cout << "  5: Sell your car to a dealership\n";
        std::cout << "-- Dealership actions --\n";
        std::cout << "  10: View all dealerships\n";
        std::cout << "  11: View a dealership's info\n";
        std::cout << "  12: View a dealership's cars\n";
        std::cout << "  13: Buy a dealership's car\n";
        std::cout << "  14: Manufacture a car for the dealership\n";
        std::cout << "-- System actions --\n";
        std::cout << "  -2: Delete user account data\n";
        std::cout << "  -5: Create vehicle dealership\n";
        std::cout << "  -7: Delete vehicle dealership\n";
        std::cout << "  -10: Save\n";
        std::cout << "  -100: Quit\n";
        std::cout << "Choice: ";

        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> choice;

        switch (choice) {
            // USER ACTIONS
            case 1: {
                switchCurrentPlayerAccount();
                break;
            }
            case 2: {
                // Print user's general info
                std::cout << "Your current info: " << *playerData << "\n";
                break;
            }
            case 3: {
                // Print bank account info
                std::cout << "Your bank account info: \n" << *(playerData->bankAccount) << "\n";
                break;
            }
            case 4: {
                std::cout << "Your vehicle list: \n";
                printPointerValsWithIdx<Vehicle>(playerData->vehicles);
                if (playerData->vehicles.empty()) {
                    std::cout << "  You have no vehicles :(\n";
                }
                break;
            }
            case 5: {
                if (playerData->vehicles.empty()) {
                    std::cout << "You cannot sell your vehicle when you don't have any. Please buy a vehicle and try again.\n";
                    break;
                }

                if (dealerships.empty()) {
                    std::cout << "You cannot sell your vehicle when no dealerships exist. Please make a dealership and try again.\n";
                    break;
                }

                std::cout << "Your vehicles:" << ": \n";
                for (int i = 0; i < playerData->vehicles.size(); i++) {
                    std::cout << i + 1 << ": " << playerData->vehicles[i]->getName() << ", $" << formatWithCommas(
                            playerData->vehicles[i]->getPrice()) << "\n";
                }

                int vehicleIdx;
                do {
                    std::cout << "\n" << "Which vehicle would you like to sell? (give index) ";
                    std::cin >> vehicleIdx;
                    vehicleIdx--;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (std::cin.fail() || vehicleIdx >= playerData->vehicles.size() || vehicleIdx < 0);

                std::cout << "List of all dealerships: \n";
                for (int i = 0; i < dealerships.size(); i++) {
                    std::cout << "  " << i + 1 << ". " << dealerships[i]->getName() << "\n";
                }

                int dealershipIdx;
                do {
                    std::cout << "\n" << "Which dealership would you like to sell your vehicle to? (give index) ";
                    std::cin >> dealershipIdx;
                    dealershipIdx--;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (std::cin.fail() || dealershipIdx >= dealerships.size() || dealershipIdx < 0);

                int soldVehicleIdx = dealerships[dealershipIdx]->sellVehicleTo(playerData->vehicles[vehicleIdx], playerData->bankAccount);
                if (soldVehicleIdx == -1) {
                    std::cout << "The transaction could not be completed, likely because the dealership doesn't have enough money or due to deposit/withdraw limits on either accounts. Please check these and try again later.\n";
                    break;
                }

                playerData->vehicles.erase(playerData->vehicles.begin() + vehicleIdx);

                std::cout << "The transaction was successful! " << dealerships[dealershipIdx]->getName() << " now owns this vehicle:\n" << *dealerships[dealershipIdx]->vehicles[soldVehicleIdx] << "\n";

                break;
            }

            // DEALERSHIP ACTIONS
            case 10: {
                std::cout << "List of all dealerships: \n";
                for (int i = 0; i < dealerships.size(); i++) {
                    std::cout << "  " << i + 1 << ". " << dealerships[i]->getName() << "\n";
                }
                if (dealerships.empty()) {
                    std::cout << "  There are no dealerships :(\n";
                }
                break;
            }
            case 11: {
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to view.\n";
                    break;
                }

                int idx;
                do {
                    std::cout << "\n" << "Which dealership would like to view more info about? (give index) ";
                    std::cin >> idx;
                    idx--;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (std::cin.fail() || idx >= dealerships.size() || idx < 0);

                std::cout << *dealerships[idx] << "\n";
                break;
            }
            case 12: {
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to view.\n";
                    break;
                }

                int idx;
                do {
                    std::cout << "\n" << "Which dealership's vehicles would you like to view? (give index) ";
                    std::cin >> idx;
                    idx--;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (std::cin.fail() || idx >= dealerships.size() || idx < 0);

                std::cout << "Vehicles of " << dealerships[idx]->getName() << ": \n";
                for (int i = 0; i < dealerships[idx]->vehicles.size(); i++) {
                    std::cout << i + 1 << ": " << *dealerships[idx]->vehicles[i] << "\n";
                }
                if (dealerships.empty()) {
                    std::cout << "  There are no vehicles :(\n";
                }

                break;
            }
            case 13: {
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to view.\n";
                    break;
                }

                int dealershipIdx;
                do {
                    std::cout << "\n" << "Which dealership's vehicles would like to buy? (give index) ";
                    std::cin >> dealershipIdx;
                    dealershipIdx--;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (std::cin.fail() || dealershipIdx >= dealerships.size() || dealershipIdx < 0);

                std::cout << "Vehicles of " << dealerships[dealershipIdx]->getName() << ": \n";
                for (int i = 0; i < dealerships[dealershipIdx]->vehicles.size(); i++) {
                    std::cout << i + 1 << ": " << *dealerships[dealershipIdx]->vehicles[i] << "\n";
                }
                if (dealerships.empty()) {
                    std::cout << "  There are no vehicles :(\n";
                    break;
                }

                int vehicleIdx;
                do {
                    std::cout << "\n" << "Which vehicle would like to buy? (give index) ";
                    std::cin >> vehicleIdx;
                    vehicleIdx--;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (std::cin.fail() || vehicleIdx >= dealerships[dealershipIdx]->vehicles.size() || vehicleIdx < 0);

                Vehicle* boughtVehicle = dealerships[dealershipIdx]->buyVehicleFrom(vehicleIdx, playerData->bankAccount);
                if (boughtVehicle == nullptr) {
                    std::cout << "The transaction could not be completed, likely because you don't have enough money or deposit/withdraw limits on either accounts. Please check these.\n";
                    break;
                }

                playerData->vehicles.push_back(boughtVehicle);
                std::cout << "The transaction was successful! You now own this vehicle:\n" << *playerData->vehicles[playerData->vehicles.size() - 1] << "\n";
                break;
            }
            case 14: {
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to generate a vehicle for.\n";
                    break;
                }

                generateVehicleFromUserInputAndAttach();
                break;
            }

            // SYSTEM ACTIONS
            case -2: {
                int idx;
                do {
                    std::cout << "\n" << "Enter the index of the account to delete:";
                    std::cin >> idx;
                    idx--;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (std::cin.fail() || idx >= people.size() || idx < 0);

                if (people[idx] == playerData) {
                    std::cout << "You cannot delete the user that you are signed in as.\n";
                    break;
                }

                people.erase(people.begin() + idx);

                std::cout << "Successfully deleted account data.\n";
                break;
            }
            case -5: {
                dealerships.push_back(generateDealershipFromInput());
                std::cout << "Successfully created a dealership!\n";
                break;
            }
            case -7: {
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to delete.\n";
                    break;
                }

                int idx;
                do {
                    std::cout << "\n" << "Enter the index of the dealership to delete:";
                    std::cin >> idx;
                    idx--;

                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (std::cin.fail() || idx >= dealerships.size() || idx < 0);

                dealerships.erase(dealerships.begin() + idx);

                std::cout << "Successfully deleted dealership data.\n";
                break;
            }
            case -10: {
                saveAllData();
                break;
            }
            case -100: {
                std::cout << "Thank you for using the program!\n";
                return 0;
            }
            default: {
                std::cout << "Invalid input. Please try again.";
                break;
            }
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string tmp;
        std::cout << "\nPress enter to continue...";
        std::getline(std::cin, tmp);
        /*
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         */

        std::cout << "\n\n";
    }
}