#include "DocumentRepository.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace fs = std::filesystem;

static std::string runPdfToText(const std::string &pdfPath) {
    // Path to pdftotext.exe  
    // → You can configure this later through a config file or CLI option  
    std::string exePath = "C:\\tools\\poppler\\poppler-25.12.0\\Library\\bin\\pdftotext.exe";

    // Temporary output file
    std::string outTxt = pdfPath + ".txt";

    // Build command:  pdftotext.exe "file.pdf" "file.pdf.txt"
    std::string cmd = "\"" + exePath + "\" \"" + pdfPath + "\" \"" + outTxt + "\"";

    // Execute
    system(cmd.c_str());

    // Read output text
    std::ifstream infile(outTxt);
    if (!infile.is_open()) {
        std::cerr << "Failed to open text output: " << outTxt << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    infile.close();

    // Remove temp file if you want:
    // fs::remove(outTxt);

    return buffer.str();
}

bool DocumentRepository::loadDirectory(const std::string &path) {
    for (const auto &entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".pdf") {
            std::string pdfFile = entry.path().string();
            std::string text = runPdfToText(pdfFile);

            if (!text.empty()) {
                docs.push_back({ entry.path().filename().string(), text });
            }
        }
    }

    return true;
}

const std::vector<std::pair<std::string,std::string>>& 
DocumentRepository::getDocuments() const {
    return docs;
}
