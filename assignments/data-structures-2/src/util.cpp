#include "util.hpp"
#include <iomanip>
#include <locale>
#include <iostream>

using namespace std;

std::string generate_uuid_v4() {
    // Set up pseudo random number generators
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 15);
    uniform_int_distribution<> dis2(8, 11);

    stringstream ss;
    int i;
    ss << hex;
    // Generate first path of UUID
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";

    // Generate second part of UUID
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";

    // Generate third part of UUID
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";

    // Generate fourth part of UUID
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";

    // Generate fifth part of UUID
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    }

    return ss.str();
}

std::string promptFullLineWithValidation(const string &prompt, function<bool(std::string)> checker) {
    // Mostly the same as the other promptWithValidation function, except this uses std::getline instead
    std::string inp;

    // Continuously attempt to get valid input until we do
    do {
        cout << prompt;
        cin.clear();
        cin.sync();
        getline(cin, inp);

        // Error in converting input to the type, clear the buffer and try again next time
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Print out an error message if we're still going to continue prompting them
        if (cin.fail() || !checker(inp)) {
            cout << "Invalid input. Please try again.\n";
        }
    } while (std::cin.fail() || !checker(inp));

    return inp;
}