#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <string>
#include <vector>
#include "Evidence.hpp"

class Document {
public:
    Document(const std::string& name, const std::string& content);

    std::vector<Evidence> searchForKeyword(const std::string& keyword) const;

    const std::string& getName() const;

private:
    std::string filename;
    std::string text;
};

#endif // DOCUMENT_HPP
