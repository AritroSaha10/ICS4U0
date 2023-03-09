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
    std::cin >> middleName;
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
    struct tm tm{};
    time_t rawtime;
    time(&rawtime);
    tm = *localtime(&rawtime);
    tm.tm_year = birthYear - 1900;
    tm.tm_mon = birthMonth - 1;
    tm.tm_mday = birthDay;
    int64_t birthTimestamp = mktime(&tm);

    return new Person{firstName, middleName, lastName, birthTimestamp, height,
            new BankAccount( startingBalance, minBalance, withdrawLimit, depositLimit)};
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

Person* playerData;
std::vector<Person*> people;
std::vector<VehicleDealership*> dealerships;

int main() {
    std::cout << color::rize("Welcome to the car simulator!\n", "Red", "Default", "Bold");

    // Load in all people
    createDataDirs();
    std::string path = "data/people";
    for (const auto & entry : fs::directory_iterator(path))
        people.push_back(new Person(Person::loadFromPath(entry.path())));

    std::cout << color::rize("List of preloaded people:\n", "Green");
    printPointerValsWithIdx<Person>(people);



    std::string ans;
    do {
        std::cout << "Would you like to import your account (i) or create a new one (c)? ";
        std::cin >> ans;
    } while (std::cin.fail() || !(ans == "i" || ans == "c"));

    if (ans == "c") {
        people.push_back(generatePersonFromInput());
        playerData = people[0];
    } else if (ans == "i") {
        int idx;
        do {
            std::cout << "\n" << "Enter the index of the account to import:";
            std::cin >> idx;
            idx--;
        } while (std::cin.fail() || idx >= people.size());
        playerData = people[idx];
    }

    std::cout << "Your info: " << *playerData << "\n";

    /*
    while (true) {
        int choice = -1;

        std::cout << "What would you like to do?";
        std::cout << "  1: Create person\n";
        std::cout << "  0: Quit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                Person tmp = generatePersonFromInput();
                people.push_back(tmp);
                std::cout << "Person generated successfully! Info: " << tmp << "\n";
                break;
            }
            case 0: {
                std::cout << "Thank you for using the program!\n";
                return 0;
            }
            default: {
                std::cout << "Invalid input. Please try again.";
                break;
            }
        }

        std::cout << "\n\n";
    }
     */

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