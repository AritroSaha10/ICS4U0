// SOURCE: https://stackoverflow.com/a/60198074/7363404
#pragma once

#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <functional>

/**
 * Generates a UUIDv4.
 * @return a UUIDv4
 */
extern std::string generate_uuid_v4();
/*
extern void save_bank_account(BankAccount* bankAccount);
extern BankAccount load_bank_account(const std::string& uuid);
*/

/**
 * A class that is used for formatting numbers properly using commas
 */
class CommaNumPunct : public std::numpunct<char> {
protected:
    char do_thousands_sep() const override {
        return ',';
    }

    std::string do_grouping() const override {
        return "\03";
    }
};

/**
 * Formats a number to have commas every 3 digits for readability
 * @tparam T the type of the value that should be formatted with commas (generally only number types)
 * @param value the value to format
 * @return The formatted value with commas
 */
template<class T>
std::string formatWithCommas(T value) {
    std::stringstream ss;
    ss.imbue(std::locale(std::locale(), new CommaNumPunct()));
    ss << std::fixed << std::setprecision(2) << value;
    return ss.str();
}

/**
 * Prompts a user and requires valid input
 * @tparam T the type of the value requested from the user
 * @return the final value from the user
 */
template<class T>
T promptWithValidation(const std::string &prompt) {
    // Wrap the previous class, but have the checker do nothing
    return promptWithValidation(prompt, [](T, bool) { return true; });
}

/**
 * Prompts a user and requires valid input for a full length string (no splitting)
 * @param checker the checker function to further validate their input
 * @return the final value from the user
 */
std::string promptFullLineWithValidation(const std::string &prompt, std::function<bool(std::string)> checker, bool removeWhitespace = true);

/**
 * Prompts a user and requires valid input
 * @tparam T the type of the value requested from the user
 * @param prompt the prompt to ask the user
 * @param checker the checker function to further validate their input
 * @return the final value from the user
 */
template<class T>
T promptWithValidation(const std::string &prompt, std::function<bool(T)> checker) {
    T inp;

    // Continuously attempt to get valid input until we do
    bool failed;
    do {
        failed = false;
        std::cout << prompt;
        std::cin.clear();
        std::cin.sync();
        std::cin >> inp;

        // Error in converting input to the type, clear the buffer and try again next time
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            failed = true;
        }

        // Print out an error message if we're still going to continue prompting them
        if (failed || !checker(inp)) {
            std::cout << "Invalid input. Please try again.\n";
        }
    } while (failed || !checker(inp));

    return inp;
}
