// --- File: src/main.cpp ---
#include "ControlCatalog.hpp"
#include "DocumentRepository.hpp"
#include "Questionnaire.hpp"
#include "SearchEngine.hpp"
#include <iostream>

int main() {
    std::cout << "[DEBUG] Program started\n";

    std::string controlsPath = "./data/controls.json";
    std::string documentsPath = "./data/documents";

    std::cout << "[DEBUG] Initializing SearchEngine\n";
    SearchEngine searchEngine(controlsPath, documentsPath);

    std::cout << "[DEBUG] Initializing Questionnaire\n";
    Questionnaire results;

    std::cout << "[DEBUG] Prompting for input\n";
    std::string input;
    std::cout << "Enter your question: ";
    std::getline(std::cin, input);

    std::cout << "[DEBUG] Running analysis\n";
    std::vector<SearchResult> analysisResults = searchEngine.analyzeQuestion(input);

    std::cout << "[DEBUG] Analysis complete, processing results\n";
    for (const auto& r : analysisResults) {
        results.addResult(r);
    }

    std::vector<SearchResult> allResults = results.getResults();
    std::cout << "[DEBUG] Outputting results\n";
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