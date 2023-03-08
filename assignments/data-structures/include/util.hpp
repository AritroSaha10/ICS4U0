// SOURCE: https://stackoverflow.com/a/60198074/7363404
#pragma once
#include <random>
#include <sstream>
#include "BankAccount.hpp"

extern std::string generate_uuid_v4();
extern void save_bank_account(BankAccount* bankAccount);
extern BankAccount load_bank_account(const std::string& uuid);