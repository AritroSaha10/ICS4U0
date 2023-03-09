// SOURCE: https://stackoverflow.com/a/60198074/7363404
#pragma once
#include <random>
#include <sstream>
#include <iomanip>
#include "BankAccount.hpp"

extern std::string generate_uuid_v4();
extern void save_bank_account(BankAccount* bankAccount);
extern BankAccount load_bank_account(const std::string& uuid);

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

template<class T>
std::string FormatWithCommas(T value)
{
    std::stringstream ss;
    ss.imbue(std::locale(std::locale(), new CommaNumPunct()));
    ss << std::fixed << std::setprecision(2) << value;
    return ss.str();
}