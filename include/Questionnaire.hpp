#ifndef QUESTIONNAIRE_HPP
#define QUESTIONNAIRE_HPP

#include <vector>
#include "SearchEngine.hpp"

class Questionnaire {
public:
    void addResult(const SearchResult &result);
    const std::vector<SearchResult>& getResults() const;

private:
    std::vector<SearchResult> results;
};

#endif // QUESTIONNAIRE_HPP
