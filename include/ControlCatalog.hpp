#ifndef CONTROLCATALOG_HPP
#define CONTROLCATALOG_HPP

#include <string>
#include <vector>
#include <optional>

struct ControlItem {
    std::string id;
    std::string question;
    std::vector<std::string> keywords;
};

class ControlCatalog {
public:
    // Load controls.json from file
    bool loadFromFile(const std::string &path);

    // Access all controls
    const std::vector<ControlItem>& getControls() const;

    const ControlItem* getById(const std::string &id) const;

private:
    std::vector<ControlItem> controls;
};

#endif // CONTROLCATALOG_HPP
