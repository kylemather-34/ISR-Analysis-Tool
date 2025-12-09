// --- File: src/main.cpp ---
#include "ControlCatalog.hpp"
#include "DocumentRepository.hpp"
#include "Questionnaire.hpp"
#include "SearchEngine.hpp"
#include <iostream>

int main() {
    std::string controlsPath = "./data/controls.json";
    std::string documentsPath = "./data/documents.json";

    SearchEngine searchEngine(controlsPath, documentsPath);
    Questionnaire results;

    std::string input;
    std::cout << "Enter your question: ";
    std::getline(std::cin, input);

    std::vector<SearchResult> analysisResults = searchEngine.analyzeQuestion(input);
    for (const auto& r : analysisResults) {
        results.addResult(r);
    }

    std::vector<SearchResult> allResults = results.getResults();
    for (const auto& res : allResults) {
        std::cout << "Control ID: " << res.controlId << "\n";
        std::cout << "Confidence: " << res.confidence << "\n";
        for (const auto& ev : res.evidence) {
            std::cout << " - Keyword: " << ev.keyword << ", Source: " << ev.sourceFile << "\n";
            std::cout << "   Snippet: " << ev.snippet << "\n";
        }
    }

    return 0;
}
