// --- File: include/SearchEngine.hpp ---
#ifndef SEARCH_ENGINE_HPP
#define SEARCH_ENGINE_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <cmath>
#include <cctype>
#include <locale>
#include <codecvt>
#include <stdexcept>
#include "ControlCatalog.hpp"
#include "DocumentRepository.hpp"

struct Evidence {
    std::string keyword;
    std::string snippet;
    std::string sourceFile;
};

struct SearchResult {
    std::string controlId;
    float confidence;
    std::vector<Evidence> evidence;
};

class SearchEngine {
public:
    SearchEngine(const std::string& controlsPath, const std::string& documentsPath);

    std::vector<SearchResult> analyzeQuestion(const std::string& question) const;
    std::string extractSentence(const std::string& text, const std::string& keyword) const;
    size_t getDocumentCount() const { return documents.size(); }


private:
    ControlCatalog catalog;
    DocumentRepository repo;
    std::unordered_map<std::string, std::vector<std::string>> controls;
    std::unordered_map<std::string, std::string> documents;

    std::vector<std::string> tokenize(const std::string& text) const;
    std::map<std::string, int> computeTF(const std::vector<std::string>& tokens) const;
    std::map<std::string, double> computeIDF() const;
    std::map<std::string, double> computeTFIDF(const std::map<std::string, int>& tf, const std::map<std::string, double>& idf) const;
    double computeCosineSimilarity(const std::map<std::string, double>& vec1, const std::map<std::string, double>& vec2) const;
    std::string toLower(const std::string& str) const;
    std::vector<std::string> splitIntoSentences(const std::string& text) const;
};

#endif // SEARCH_ENGINE_HPP
