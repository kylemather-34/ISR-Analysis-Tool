#include "SearchEngine.hpp"
#include "ControlCatalog.hpp"
#include "DocumentRepository.hpp"
#include <numeric>
#include <sstream>
#include <map>
#include <set>
#include <cmath>
#include <cctype>

SearchEngine::SearchEngine(const std::string& controlsPath, const std::string& documentsPath) {
    std::cout << "[DEBUG] SearchEngine constructor entered\n";

    std::cout << "[DEBUG] Attempting to load controls from: " << controlsPath << "\n";
    if (!catalog.loadFromFile(controlsPath)) {
        std::cerr << "[ERROR] Failed to load controls from: " << controlsPath << "\n";
    } else {
        std::cout << "[DEBUG] Loaded controls.json with " << catalog.getControls().size() << " items\n";
    }

    for (const auto& item : catalog.getControls()) {
        controls[item.id] = item.keywords;
    }

    std::cout << "[DEBUG] Attempting to load documents from: " << documentsPath << "\n";
    if (!repo.loadDirectory(documentsPath)) {
        std::cerr << "[ERROR] Failed to load documents from: " << documentsPath << "\n";
    } else {
        std::cout << "[DEBUG] Loaded " << repo.getDocuments().size() << " documents\n";
    }

    for (const auto& [filename, content] : repo.getDocuments()) {
        documents[filename] = content;
    }

    std::cout << "[DEBUG] SearchEngine constructor finished\n";
}


std::string SearchEngine::extractSentence(const std::string& text, const std::string& keyword) const {
    std::vector<std::string> sentences = splitIntoSentences(text);
    for (const auto& sentence : sentences) {
        if (sentence.find(keyword) != std::string::npos) {
            return sentence;
        }
    }
    return "";
}

std::vector<SearchResult> SearchEngine::analyzeQuestion(const std::string& question) const {
    std::vector<SearchResult> results;
    std::vector<std::string> queryTokens = tokenize(toLower(question));
    auto queryTF = computeTF(queryTokens);
    auto idf = computeIDF();
    auto queryTFIDF = computeTFIDF(queryTF, idf);

    for (const auto& [controlId, keywords] : controls) {
        SearchResult result;
        result.controlId = controlId;
        result.confidence = 0.0f;

        for (const auto& [docId, text] : documents) {
            for (const auto& keyword : keywords) {
                std::string keywordLower = toLower(keyword);
                if (text.find(keywordLower) != std::string::npos) {
                    Evidence ev;
                    ev.keyword = keyword;
                    ev.snippet = extractSentence(text, keywordLower);
                    ev.sourceFile = docId;
                    result.evidence.push_back(ev);
                    result.confidence += 1.0f;
                }
            }
        }
        if (!result.evidence.empty()) {
            results.push_back(result);
        }
    }
    return results;
}

std::vector<std::string> SearchEngine::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::map<std::string, int> SearchEngine::computeTF(const std::vector<std::string>& tokens) const {
    std::map<std::string, int> tf;
    for (const auto& word : tokens) {
        tf[word]++;
    }
    return tf;
}

std::map<std::string, double> SearchEngine::computeIDF() const {
    std::map<std::string, double> idf;
    size_t totalDocs = documents.size();
    std::map<std::string, int> docFreq;

    for (const auto& [_, text] : documents) {
        std::set<std::string> seen;
        for (const auto& word : tokenize(toLower(text))) {
            seen.insert(word);
        }
        for (const auto& word : seen) {
            docFreq[word]++;
        }
    }

    for (const auto& [word, df] : docFreq) {
        idf[word] = log(static_cast<double>(totalDocs) / (1 + df));
    }

    return idf;
}

std::map<std::string, double> SearchEngine::computeTFIDF(
    const std::map<std::string, int>& tf,
    const std::map<std::string, double>& idf) const {

    std::map<std::string, double> tfidf;
    for (const auto& [word, freq] : tf) {
        auto it = idf.find(word);
        if (it != idf.end()) {
            tfidf[word] = freq * it->second;
        }
    }
    return tfidf;
}

double SearchEngine::computeCosineSimilarity(
    const std::map<std::string, double>& vec1,
    const std::map<std::string, double>& vec2) const {

    double dot = 0.0, normA = 0.0, normB = 0.0;
    for (const auto& [word, val] : vec1) {
        dot += val * (vec2.count(word) ? vec2.at(word) : 0.0);
        normA += val * val;
    }
    for (const auto& [_, val] : vec2) {
        normB += val * val;
    }
    return normA && normB ? dot / (sqrt(normA) * sqrt(normB)) : 0.0;
}

std::string SearchEngine::toLower(const std::string& str) const {
    std::string result;
    for (char c : str) {
        result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

std::vector<std::string> SearchEngine::splitIntoSentences(const std::string& text) const {
    std::vector<std::string> sentences;
    std::istringstream iss(text);
    std::string sentence;
    while (std::getline(iss, sentence, '.')) {
        if (!sentence.empty()) {
            sentences.push_back(sentence);
        }
    }
    return sentences;
}