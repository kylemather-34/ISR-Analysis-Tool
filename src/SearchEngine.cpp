#include "SearchEngine.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>

// Utility: lowercase a string
static std::string toLower(const std::string &s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return out;
}

// Extract the sentence around the keyword position
std::string SearchEngine::extractSentence(const std::string &text, size_t pos) const {
    if (pos >= text.size()) return "";

    size_t start = text.rfind('.', pos);
    size_t end   = text.find('.', pos);

    if (start == std::string::npos) start = 0;
    else start++;

    if (end == std::string::npos) end = text.size();

    std::string snippet = text.substr(start, end - start);

    // Trim whitespace
    snippet.erase(snippet.begin(), std::find_if(snippet.begin(), snippet.end(),
        [](unsigned char c){ return !std::isspace(c); }));

    snippet.erase(std::find_if(snippet.rbegin(), snippet.rend(),
        [](unsigned char c){ return !std::isspace(c); }).base(), snippet.end());

    return snippet;
}

std::vector<SearchResult> SearchEngine::analyze(
    const ControlCatalog &catalog,
    const DocumentRepository &repo)
{
    std::vector<SearchResult> results;

    auto &controls = catalog.getControls();
    auto &documents = repo.getDocuments();

    for (const auto &control : controls)
    {
        SearchResult result;
        result.controlId = control.id;
        int totalKeywords = control.keywords.size();
        int keywordHits = 0;

        for (const auto &[fileName, text] : documents)
        {
            std::string lowerText = toLower(text);

            for (const auto &kw : control.keywords)
            {
                std::string lowerKw = toLower(kw);

                size_t pos = lowerText.find(lowerKw);
                if (pos != std::string::npos)
                {
                    keywordHits++;

                    Evidence ev;
                    ev.keyword = kw;
                    ev.snippet = extractSentence(text, pos);
                    ev.sourceFile = fileName;

                    result.evidence.push_back(ev);
                }
            }
        }

        result.confidence =
            (totalKeywords == 0) ? 0.0 :
            static_cast<double>(keywordHits) / static_cast<double>(totalKeywords);

        results.push_back(result);
    }

    return results;
}
