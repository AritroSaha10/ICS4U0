// SOURCE: https://stackoverflow.com/a/60198074/7363404
#pragma once
#include <random>
#include <sstream>
#include <iomanip>
#include "BankAccount.hpp"

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
class CommaNumPunct : public std::numpunct<char>
{
protected:
    char do_thousands_sep() const override
    {
        return ',';
    }

    std::string do_grouping() const override
    {
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
std::string FormatWithCommas(T value)
{
    std::stringstream ss;
    ss.imbue(std::locale(std::locale(), new CommaNumPunct()));
    ss << std::fixed << std::setprecision(2) << value;
    return ss.str();
}