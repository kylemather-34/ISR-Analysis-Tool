#ifndef SEARCHENGINE_HPP
#define SEARCHENGINE_HPP

#include <string>
#include <vector>
#include <utility>

#include "ControlCatalog.hpp"
#include "DocumentRepository.hpp"
#include "Questionnaire.hpp"

struct Evidence {
    std::string keyword;
    std::string snippet;
    std::string sourceFile;
};

struct SearchResult {
    std::string controlId;
    double confidence;
    std::vector<Evidence> evidence;
};

class SearchEngine {
public:
    // Analyze documents against the control catalog
    std::vector<SearchResult> analyze(
        const ControlCatalog &catalog,
        const DocumentRepository &repo);

private:
    std::string extractSentence(const std::string &text, size_t pos) const;
};

#endif // SEARCHENGINE_HPP
