#include "Document.hpp"
#include <algorithm>

Document::Document(const std::string& name, const std::string& content)
    : filename(name), text(content) {}

const std::string& Document::getName() const {
    return filename;
}

std::vector<Evidence> Document::searchForKeyword(const std::string& keyword) const {
    std::vector<Evidence> results;

    size_t pos = text.find(keyword);
    while (pos != std::string::npos) {
        size_t snippetStart = (pos > 50) ? pos - 50 : 0;
        size_t snippetEnd = std::min(pos + 50, text.size() - 1);
        std::string snippet = text.substr(snippetStart, snippetEnd - snippetStart + 1);

        Evidence ev;
        ev.keyword = keyword;
        ev.sourceFile = filename;
        ev.snippet = snippet;
        results.push_back(ev);


        pos = text.find(keyword, pos + keyword.length());
    }

    return results;
}
