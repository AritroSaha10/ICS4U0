/**
 * Name: Car Simulator
 * Description: The main file for the Extending Data Structures assignment (ICS4U0). The program is a car simulator
 * that allows users to run actions on both people and vehicle dealerships. It does this using a custom-made CLI
 * interface, prompting the user which out of the 14 available actions they'd like to run and running it
 * immediately after. It also has the ability to save its current state into JSON files in a data folder.
 * This allows users to use the program at a later time without losing their data.
 *
 * @cite Feng Wang, Colorize, (2020), GitHub repository, https://github.com/fengwang/colorize
 * @cite Niels Lohmann, JSON for Modern C++, (2022), https://github.com/nlohmann/json
 *
 * @author Aritro Saha
 * Last edited: March 10, 2023
 */

#include <iostream>
#include <filesystem>
#include "BankAccount.hpp"
#include "Person.hpp"
#include "VehicleDealership.hpp"
#include "util.hpp"
#include "colorize.h"

namespace fs = std::filesystem;
using namespace std::chrono;

Person *playerData;
std::vector<Person *> people;
std::vector<VehicleDealership *> dealerships;

/**
 * Print a list of values of pointers in order with nice formatting
 * @tparam T the type used in the vector
 * @param vec vector to get pointers from
 */
template<class T>
void printPointerValuesWithIdx(std::vector<T *> &vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << i + 1 << ". " << *vec[i] << "\n";
    }
}

/**
 * Generate a Person pointer from user input.
 * @return the generated Person
 */
Person *generatePersonFromInput() {
    // Person-specific details
    auto firstName = promptFullLineWithValidation("First name: ", [](const std::string &str) { return !str.empty(); });
    auto middleName = promptFullLineWithValidation("Middle name: ", [](const std::string &) { return true; });
    auto lastName = promptFullLineWithValidation("Last name: ", [](const std::string &str) { return !str.empty(); });

    auto birthYear = promptWithValidation<int>("Birth year: ", [](int x) { return x >= 1900; });
    auto birthMonth = promptWithValidation<int>("Birth month: ", [](int x) { return x >= 1 && x <= 12; });
    auto birthDay = promptWithValidation<int>("Birth day: ", [](int x) { return x >= 1 && x <= 31; });

    auto height = promptWithValidation<double>("Height (cm): ", [](int x) { return x >= 0; });

    // Bank account details
    std::cout << color::rize("- Banking details -\n", "White", "Blue");
    auto minBalance = promptWithValidation<double>("Minimum balance: ", [](double x) { return x >= 0; });
    auto startingBalance = promptWithValidation<double>("Starting balance: ",
                                                        [&minBalance](double x) { return x >= minBalance; });
    auto withdrawLimit = promptWithValidation<double>("Withdraw limit: ", [](double x) { return x >= 0; });
    auto depositLimit = promptWithValidation<double>("Deposit limit: ", [](double x) { return x >= 0; });

    // Convert birthday to timestamp
    struct tm myTm{};
    time_t rawTime;
    time(&rawTime);
    myTm = *localtime(&rawTime);
    myTm.tm_year = birthYear - 1900;
    myTm.tm_mon = birthMonth - 1;
    myTm.tm_mday = birthDay;
    int64_t birthTimestamp = mktime(&myTm);

    return new Person{firstName, middleName, lastName, birthTimestamp, height,
                      new BankAccount(startingBalance, minBalance, withdrawLimit, depositLimit)};
}

/**
 * Generate a VehicleDealership pointer from user input.
 * @return the generated VehicleDealership
 */
VehicleDealership *generateDealershipFromInput() {
    std::cout << color::rize("-- Create a vehicle dealership --\n", "White", "Green");

    // Dealership-specific details
    auto name = promptFullLineWithValidation("Name of dealership: ",
                                             [](const std::string &str) { return !str.empty(); });

    // Bank account details
    std::cout << color::rize("- Banking details -\n", "White", "Blue");
    auto minBalance = promptWithValidation<double>("Minimum balance: ", [](double x) { return x >= 0; });
    auto startingBalance = promptWithValidation<double>("Starting balance: ",
                                                        [&minBalance](double x) { return x >= minBalance; });
    auto withdrawLimit = promptWithValidation<double>("Withdraw limit: ", [](double x) { return x >= 0; });
    auto depositLimit = promptWithValidation<double>("Deposit limit: ", [](double x) { return x >= 0; });

    // Generate a new instance of VehicleDealership with this info
    return new VehicleDealership{name, new BankAccount(startingBalance, minBalance, withdrawLimit, depositLimit)};
}

/**
 * Generate a Vehicle pointer and give it to a dealership through user input
 * @return Pointer to the generated Vehicle
 */
Vehicle *generateVehicleFromUserInputAndAttach() {
    if (dealerships.empty()) {
        std::cout
                << "ERROR: You cannot generate a vehicle without any vehicle dealerships. Please make one before making a vehicle.";
        return nullptr;
    }

    // Header for this section
    std::cout << color::rize("-- Create a vehicle --\n", "White", "Green");

    // Get all necessary info
    auto name = promptFullLineWithValidation("Complete name of vehicle (ex. Honda Accord 2023 Touring Hybrid): ",
                                             [](const std::string &str) { return !str.empty(); });
    auto price = promptWithValidation<double>("Price of vehicle: $", [](double x) { return x >= 0; });
    auto manufacturer = promptFullLineWithValidation("Manufacturer: ",
                                                     [](const std::string &str) { return !str.empty(); });
    auto wheels = promptWithValidation<int>("# of wheels on vehicle: ", [](int x) { return x > 0; });
    auto doors = promptWithValidation<int>("# of doors on vehicle: ", [](int x) { return x > 0; });
    auto seats = promptWithValidation<int>("# of seats in vehicle: ", [](int x) { return x > 0; });
    auto maxPassengers = promptWithValidation<int>("Maximum # of passengers in vehicle: ",
                                                   [](int x) { return x >= 0; });
    auto mileage = promptWithValidation<double>("Starting mileage on vehicle (km): ", [](double x) { return x >= 0; });
    auto horsepower = promptWithValidation<double>("Horsepower available: ", [](double x) { return x >= 0; });
    auto maxSpeed = promptWithValidation<double>("Max speed available (km/h): ", [](double x) { return x >= 0; });
    auto color = promptFullLineWithValidation("Color (ex. Canyon River Blue Metallic): ",
                                              [](const std::string &str) { return !str.empty(); });

    // Create a vehicle from info
    auto vehicle = new Vehicle(name, price, wheels, doors, seats, maxPassengers, manufacturer, mileage, horsepower,
                               maxSpeed, color);

    // Assign the new vehicle to a dealership
    std::cout << "Vehicle successfully created: " << *vehicle << "\n";
    std::cout << "Please choose a dealership to add it under.\n";
    for (int i = 0; i < dealerships.size(); i++) {
        std::cout << i + 1 << ". " << dealerships[i]->getName() << "\n";
    }

    auto idx = promptWithValidation<int>("Enter the index of the dealership to add the vehicle to: ",
                                         [](int x) { return x > 0 && x <= dealerships.size(); }) - 1;
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
    // Print out header for this section
    std::cout << color::rize("List of preloaded people:\n", "Green");
    printPointerValuesWithIdx<Person>(people);
    if (people.empty()) {
        std::cout
                << "  No people profiles could be loaded in :( If you didn't expect this, check the path of your data files (should be data/people)\n";
    }

    auto ans = promptFullLineWithValidation(
            "Would you like to import your account (i, will error if none are imported), or create a new one (c)? ",
            [](const std::string &str) { return (str == "i" && !people.empty()) || str == "c"; });
    if (ans == "c") {
        // Create a new person and assign them as the current player
        people.push_back(generatePersonFromInput());
        playerData = people[people.size() - 1];
    } else if (ans == "i") {
        // Choose an account from the ones given
        int idx = promptWithValidation<int>("Enter the index of the account to import: ",
                                            [](int i) { return i > 0 && i <= people.size(); }) - 1;
        playerData = people[idx];
    }

    std::cout << "Your info: " << *playerData << "\n";
}

/**
 * Serialized and save all the Person and VehicleDealership data in data/
 */
void saveAllData() {
    // Save all data for people
    for (auto person: people) {
        person->saveAsFile();
    }

    // Save all data for dealerships
    for (auto dealership: dealerships) {
        dealership->saveAsFile();
    }

    std::cout << "Saved all data!\n";
}

int main() {
    // Header to start the program off
    std::cout << color::rize(R"(
   _____           _____ _
  / ____|         / ____(_)
 | |     __ _ _ _| (___  _ _ __ ___
 | |    / _` | '__\___ \| | '_ ` _ \
 | |___| (_| | |  ____) | | | | | | |
  \_____\__,_|_| |_____/|_|_| |_| |_|
)",
                             "Orange",
                             "Default",
                             "Bold");

    std::cout << color::rize("Welcome to the Car Simulator!\n", "Red", "Default", "Bold");

    // Load in all people
    createDataDirs();
    std::string peopleDataPath = "data/people";
    for (const auto &entry: fs::directory_iterator(peopleDataPath)) {
        people.push_back(new Person(Person::loadFromPath(entry.path().string())));
    }
    // Load in all dealerships
    std::string dealershipsDataPath = "data/vehicle-dealership";
    for (const auto &entry: fs::directory_iterator(dealershipsDataPath))
        dealerships.push_back(new VehicleDealership(VehicleDealership::loadFromPath(entry.path().string())));

    // Have the player choose an account / person profile before doing anything
    switchCurrentPlayerAccount();
    std::cout << "\n\n";

    while (true) {
        // Print all the available commands
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

        int choice = promptWithValidation<int>("Choice: ", [](int) { return true; });
        switch (choice) {
            // USER ACTIONS
            case 1: {
                // Switch the player profile / account
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
                // Print out a list of the user's vehicles
                std::cout << "Your vehicle list: \n";
                printPointerValuesWithIdx<Vehicle>(playerData->vehicles);
                if (playerData->vehicles.empty()) {
                    std::cout << "  You have no vehicles :(\n";
                }
                break;
            }
            case 5: {
                // Allow the user to sell their car to a dealership if possible

                // Pre-requisite checking before allowing them to sell
                if (playerData->vehicles.empty()) {
                    std::cout
                            << "You cannot sell your vehicle when you don't have any. Please buy a vehicle and try again.\n";
                    break;
                }
                if (dealerships.empty()) {
                    std::cout
                            << "You cannot sell your vehicle when no dealerships exist. Please make a dealership and try again.\n";
                    break;
                }

                // List all the vehicles, specifically their name and price
                std::cout << "Your vehicles:" << ": \n";
                for (int i = 0; i < playerData->vehicles.size(); i++) {
                    std::cout << i + 1 << ": " << playerData->vehicles[i]->getName() << ", $" << formatWithCommas(
                            playerData->vehicles[i]->getPrice()) << "\n";
                }
                // Get which vehicle the user wants to sell
                int vehicleIdx = promptWithValidation<int>("Which vehicle would you like to sell? (give index) ",
                                                           [](int i) {
                                                               return i > 0 && i <= playerData->vehicles.size();
                                                           }) - 1;

                // List all the dealerships out
                std::cout << "List of all dealerships: \n";
                for (int i = 0; i < dealerships.size(); i++) {
                    std::cout << "  " << i + 1 << ". " << dealerships[i]->getName() << "\n";
                }
                // Get which dealership the user wants to sell their vehicle to
                int dealershipIdx = promptWithValidation<int>(
                        "Which dealership would you like to sell your vehicle to? (give index) ",
                        [](int i) { return i > 0 && i <= dealerships.size(); }) - 1;

                // Attempt to sell the vehicle to the dealership, handle if impossible
                int soldVehicleIdx = dealerships[dealershipIdx]->sellVehicleTo(playerData->vehicles[vehicleIdx],
                                                                               playerData->bankAccount);
                if (soldVehicleIdx == -1) {
                    std::cout
                            << "The transaction could not be completed, likely because the dealership doesn't have enough money or due to deposit/withdraw limits on either accounts. Please check these and try again later.\n";
                    break;
                }

                // Sell was successful, remove the vehicle from the player's list since we don't own it anymore
                playerData->vehicles.erase(playerData->vehicles.begin() + vehicleIdx);
                std::cout << "The transaction was successful! " << dealerships[dealershipIdx]->getName()
                          << " now owns this vehicle:\n" << *dealerships[dealershipIdx]->vehicles[soldVehicleIdx]
                          << "\n";

                break;
            }

                // DEALERSHIP ACTIONS
            case 10: {
                // Print out the names of all the dealerships
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
                // Print out info for a specific dealership
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to view.\n";
                    break;
                }

                int idx =
                        promptWithValidation<int>("Which dealership would like to view more info about? (give index)  ",
                                                  [](int x) { return x > 0 && x <= dealerships.size(); }) - 1;
                std::cout << *dealerships[idx] << "\n";
                break;
            }
            case 12: {
                // View all of a dealership's vehicles
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to view.\n";
                    break;
                }

                int idx = promptWithValidation<int>("Which dealership's vehicles would you like to view? (give index) ",
                                                    [](int x) { return x > 0 && x <= dealerships.size(); }) - 1;

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
                // Allow the user to buy a vehicle from a dealership
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to view.\n";
                    break;
                }

                int dealershipIdx =
                        promptWithValidation<int>("Which dealership's vehicles would like to buy? (give index) ",
                                                  [](int x) { return x > 0 && x <= dealerships.size(); }) - 1;

                std::cout << "Vehicles of " << dealerships[dealershipIdx]->getName() << ": \n";
                for (int i = 0; i < dealerships[dealershipIdx]->vehicles.size(); i++) {
                    std::cout << i + 1 << ": " << *dealerships[dealershipIdx]->vehicles[i] << "\n";
                }
                if (dealerships[dealershipIdx]->vehicles.empty()) {
                    std::cout << "  There are no vehicles :(\n";
                    break;
                }

                int vehicleIdx = promptWithValidation<int>("Which vehicle would like to buy? (give index) ",
                                                           [&dealershipIdx](int x) {
                                                               return x > 0 &&
                                                                      x <= dealerships[dealershipIdx]->vehicles.size();
                                                           }) - 1;

                Vehicle *boughtVehicle = dealerships[dealershipIdx]->buyVehicleFrom(vehicleIdx,
                                                                                    playerData->bankAccount);
                if (boughtVehicle == nullptr) {
                    std::cout
                            << "The transaction could not be completed, likely because you don't have enough money or deposit/withdraw limits on either accounts. Please check these.\n";
                    break;
                }

                playerData->vehicles.push_back(boughtVehicle);
                std::cout << "The transaction was successful! You now own this vehicle:\n"
                          << *playerData->vehicles[playerData->vehicles.size() - 1] << "\n";
                break;
            }
            case 14: {
                // Generate a new vehicle for a dealership

                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to generate a vehicle for.\n";
                    break;
                }

                generateVehicleFromUserInputAndAttach();
                break;
            }

                // SYSTEM ACTIONS
            case -2: {
                // Delete a person profile / account
                int idx = promptWithValidation<int>("Enter the index of the account to delete: ",
                                                    [](int x) { return x > 0 && x <= people.size(); }) - 1;

                if (people[idx] == playerData) {
                    std::cout << "You cannot delete the user that you are signed in as.\n";
                    break;
                }

                people.erase(people.begin() + idx);

                std::cout << "Successfully deleted account data.\n";
                break;
            }
            case -5: {
                // Generate a new dealership
                dealerships.push_back(generateDealershipFromInput());
                std::cout << "Successfully created a dealership!\n";
                break;
            }
            case -7: {
                // Delete a dealership
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to delete.\n";
                    break;
                }

                int idx = promptWithValidation<int>("Enter the index of the dealership to delete: ",
                                                    [](int x) { return x > 0 && x <= dealerships.size(); }) - 1;
                dealerships.erase(dealerships.begin() + idx);

                std::cout << "Successfully deleted dealership data.\n";
                break;
            }
            case -10: {
                // Save all the data currently in memory into the data folder
                saveAllData();
                break;
            }
            case -100: {
                // Exit from the program
                std::cout << "Thank you for using the program!\n";
                return 0;
            }
            default: {
                std::cout << "Invalid input. Please try again.";
                break;
            }
        }

        promptFullLineWithValidation("\nPress enter to continue...", [](const std::string &) { return true; });
        std::cout << "\n\n";
    }
}