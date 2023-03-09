#include <iostream>
#include "BankAccount.hpp"
#include "Person.hpp"
#include "Sedan.hpp"
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

void testSerializationBankAccount() {
    BankAccount bankAccount(100, 0, 2500, 2500);
    std::cout << "Saving this bank account: " << bankAccount.serializeToJSON() << "\n";
    std::string uuid = bankAccount.getUUID();
    bankAccount.saveAsFile();

    BankAccount newBankAccount = BankAccount::loadFromUUID(uuid);
    std::cout << "Loaded JSON: " << bankAccount.serializeToJSON() << "\n";
}

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

VehicleDealership* generateDealershipFromInput() {
    std::cout << color::rize("-- Create a vehicle dealership --\n", "White", "Green");
    std::string name;
    // Bank account details
    double startingBalance, minBalance, withdrawLimit, depositLimit;

    std::cout << "Name of vehicle dealership: ";
    std::getline(std::cin, name);
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
    if (!fs::is_directory("data/dealership") || !fs::exists("data/dealership")) {
        fs::create_directory("data/dealership");
    }
}

template <class T>
void printListWithIdx(std::vector<T> &vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << i + 1 << ". " << vec[i] << "\n";
    }
}

template <class T>
void printPointerValsWithIdx(std::vector<T*> &vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << i + 1 << ". " << *vec[i] << "\n";
    }
}

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
        } while (std::cin.fail() || idx >= people.size());
        playerData = people[idx];
    }

    std::cout << "Your info: " << *playerData << "\n";
}

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
        std::cout << entry.path().string() << "\n";
        people.push_back(new Person(Person::loadFromPath(entry.path().string())));
    }
    // Load in all dealerships
    std::string dealershipsDataPath = "data/dealership";
    for (const auto & entry : fs::directory_iterator(dealershipsDataPath))
        dealerships.push_back(new VehicleDealership(VehicleDealership::loadFromPath(entry.path().string())));

    switchCurrentPlayerAccount();

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
        std::cout << "-- System actions --\n";
        std::cout << "  -2: Delete user account data\n";
        std::cout << "  -5: Create vehicle dealership\n";
        std::cout << "  -6: Delete vehicle dealership\n";
        std::cout << "  -10: Save\n";
        std::cout << "  -100: Quit\n";
        std::cout << "Choice: ";

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                std::cout << "NOT IMPLEMENTED YET\n";
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

            // SYSTEM ACTIONS
            case -2: {
                int idx;
                do {
                    std::cout << "\n" << "Enter the index of the account to delete:";
                    std::cin >> idx;
                    idx--;
                } while (std::cin.fail() || idx >= people.size());

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
            case -6: {
                if (dealerships.empty()) {
                    std::cout << "There are no dealerships to delete.\n";
                    break;
                }

                int idx;
                do {
                    std::cout << "\n" << "Enter the index of the dealership to delete:";
                    std::cin >> idx;
                    idx--;
                } while (std::cin.fail() || idx >= dealerships.size());

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

        std::cout << "\n";
    }

    /*
    // testSerializationBankAccount();
    // VehicleDealership hondaDealership = VehicleDealership::loadFromUUID("3f4294f0-451a-474d-9912-823672c66b6f");
    VehicleDealership hondaDealership("Honda Dealership", new BankAccount(0, 1000000, 1000000));
    Sedan sedan1("Honda Accord", 50000.0, "Honda", 0, 200, 300, 50, 20, "Green");
    Sedan sedan2("Honda Civic", 35042.0, "Honda", 0, 150, 175, 20, 20, "Grey");
    hondaDealership.giveVehicle(&sedan1);
    hondaDealership.giveVehicle(&sedan2);

    std::string personUUID;
    std::cout << "UUID of person: ";
    std::cin >> personUUID;
    // Person p1 = Person::loadFromUUID(personUUID);
    Person p1("Aritro", "", "Saha", 0, 200, new BankAccount(0, 10e10, 10e10));
    std::cout << "Person 1: " << p1 << "\n";
    p1.bankAccount->deposit(100000);

    // std::cout << sedan1.serializeToJSON() << "\n\n";

    for (Vehicle *vehicle: hondaDealership.getVehicles()) {
        std::cout << vehicle->getName() << " - $" << vehicle->getPrice() << " - " << vehicle->serializeToJSON()
                  << "\n\n";

        Vehicle *boughtVehicle = hondaDealership.buyVehicleFrom(0, p1.bankAccount);
        if (boughtVehicle == nullptr) {
            std::cout << "Something went wrong when buying the vehicle\n";
            return 1;
        }
        p1.vehicles.push_back(boughtVehicle);
    }

    for (Vehicle *vehicle: p1.vehicles) {
        std::cout << vehicle->getName() << " - $" << vehicle->getPrice() << " - " << vehicle->serializeToJSON()
                  << "\n\n";
    }

    p1.saveAsFile();
    hondaDealership.saveAsFile();
    */


    return 0;
}