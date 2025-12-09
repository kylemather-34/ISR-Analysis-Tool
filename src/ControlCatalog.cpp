#include "ControlCatalog.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

bool ControlCatalog::loadFromFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open controls file: " << path << "\n";
        return false;
    }

    nlohmann::json j;
    file >> j;

    for (auto &item : j) {
        ControlItem c;
        c.id = item["id"].get<std::string>();
        c.question = item["question"].get<std::string>();
        c.keywords = item["keywords"].get<std::vector<std::string>>();
        controls.push_back(c);
    }

    return true;
}

const std::vector<ControlItem>& ControlCatalog::getControls() const {
    return controls;
}