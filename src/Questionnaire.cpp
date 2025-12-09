#include "Questionnaire.hpp"

void Questionnaire::addResult(const SearchResult &result) {
    results.push_back(result);
}

const std::vector<SearchResult>& Questionnaire::getResults() const {
    return results;
}
