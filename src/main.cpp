#include <iostream>
#include <string>
#include <filesystem>

#include "ControlCatalog.hpp"
#include "DocumentRepository.hpp"
#include "SearchEngine.hpp"
#include "Questionnaire.hpp"

// ANSI colors for Windows Terminal (VS Code, Windows Terminal, Git Bash)
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_RED     "\033[31m"

namespace fs = std::filesystem;

std::string colorForConfidence(double c) {
    if (c >= 0.75) return COLOR_GREEN;
    if (c >= 0.40) return COLOR_YELLOW;
    return COLOR_RED;
}

int main(int argc, char** argv) {
    std::cout << "=== ISR Analysis Tool ===\n";

    // Determine policy directory (default: ./policies)
    std::string policyDir = "policies";
    if (argc > 1) {
        policyDir = argv[1];
    }

    if (!fs::exists(policyDir)) {
        std::cerr << "ERROR: The directory '" << policyDir << "' does not exist.\n";
        return 1;
    }

    // --------------------------
    // Load the NIST control catalog
    // --------------------------
    ControlCatalog catalog;
    if (!catalog.loadFromFile("data/controls.json")) {
        std::cerr << "ERROR: Could not load controls.json\n";
        return 1;
    }

    std::cout << "Loaded " << catalog.getControls().size() << " controls.\n";

    // --------------------------
    // Load policy documents
    // --------------------------
    DocumentRepository repo;
    repo.loadDirectory(policyDir);

    if (repo.getDocuments().empty()) {
        std::cerr << "ERROR: No PDF documents found in '" << policyDir << "'\n";
        return 1;
    }

    std::cout << "Loaded " << repo.getDocuments().size()
              << " document(s) from directory: " << policyDir << "\n";

    // --------------------------
    // Run Search Engine
    // --------------------------
    SearchEngine engine;
    Questionnaire results;

    auto analysisResults = engine.analyze(catalog, repo);

    for (const auto &r : analysisResults) {
        results.addResult(r);
    }

    // --------------------------
    // Print results
    // --------------------------
    std::cout << "\n=== Analysis Report ===\n\n";

    for (const auto &res : results.getResults()) {
        std::string color = colorForConfidence(res.confidence);

        std::cout << color << "Control: " << res.controlId << COLOR_RESET << "\n";
        std::cout << "Confidence: " << res.confidence << "\n";

        if (res.evidence.empty()) {
            std::cout << COLOR_RED << "  No evidence found.\n" << COLOR_RESET;
        } else {
            std::cout << "Evidence:\n";
            for (const auto &ev : res.evidence) {
                std::cout << "  - [" << ev.keyword << "] (" << ev.sourceFile << ")\n"
                          << "      \"" << ev.snippet << "\"\n";
            }
        }

        std::cout << "---------------------------------------------\n";
    }

    std::cout << "\nAnalysis complete.\n";
    return 0;
}
