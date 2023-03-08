#include "util.hpp"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;


std::string generate_uuid_v4() {
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    return ss.str();
}

void save_bank_account(BankAccount* bankAccount) {
    json serializedJSON = bankAccount->serializeToJSON();

    // Make data directory if needed
    if (!fs::is_directory("data") || !fs::exists("data")) { // Check if folder exists
        fs::create_directory("data");
    }

    // Make bank accounts directory if needed
    if (!fs::is_directory("data/bank_accounts") || !fs::exists("data/bank_accounts")) {
        fs::create_directory("data/bank_accounts");
    }

    // Write data to file
    std::ofstream file("data/bank_accounts/" + bankAccount->getUUID() + ".json");
    file << std::setw(4) << serializedJSON << std::endl;
    file.close();
}

BankAccount load_bank_account(const std::string& uuid) {
    std::string fname = "data/bank_accounts/" + uuid + ".json";
    if (!fs::exists(fname)) {
        // File needs to exist to read anything
        throw;
    }

    std::ifstream file(fname);
    json importedJSON;
    file >> importedJSON;

    return BankAccount::deserializeFromJSON(importedJSON);
}