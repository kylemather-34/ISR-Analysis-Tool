#ifndef QUESTIONNAIRE_HPP
#define QUESTIONNAIRE_HPP

#include <vector>
#include "SearchEngine.hpp"

class Questionnaire {
public:
    void addResult(const SearchResult &result);
    const std::vector<SearchResult>& getResults() const;
    bool empty() const { return results.empty(); }

private:
    std::vector<SearchResult> results;
};

#endif // QUESTIONNAIRE_HPP
