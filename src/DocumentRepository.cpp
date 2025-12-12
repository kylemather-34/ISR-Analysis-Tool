#include "DocumentRepository.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>

namespace fs = std::filesystem;

std::string sanitizePath(const std::string& path) {
    std::string result = path;
    std::replace(result.begin(), result.end(), '/', '\\');
    return result;
}

#include <windows.h>

bool runCommand(const std::string& command) {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::wstring cmdW(command.begin(), command.end());

    if (!CreateProcessW(
        NULL,
        cmdW.data(), // command line
        NULL, NULL, FALSE, 0, NULL, NULL,
        &si, &pi)) {
        std::cerr << "[ERROR] CreateProcess failed. Error code: " << GetLastError() << "\n";
        return false;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

static std::string runPdfToText(const std::string &pdfPathStr) {
    std::string exePath = "C:\\tools\\poppler\\poppler-25.12.0\\Library\\bin\\pdftotext.exe";

    fs::path pdfPath = fs::absolute(pdfPathStr);
    fs::path txtPath = pdfPath;
    txtPath += ".txt";

    std::string cmd = "\"" + sanitizePath(exePath) + "\" \"" +
                  sanitizePath(pdfPath.string()) + "\" \"" +
                  sanitizePath(txtPath.string()) + "\"";
    std::cout << "[DEBUG] txtPath: " << txtPath << "\n";
    std::cout << "[DEBUG] Exists after conversion? " << fs::exists(txtPath) << "\n";
    std::cout << "[DEBUG] Executing: " << cmd << "\n";

    if (!runCommand(cmd)) {
        std::cerr << "[ERROR] pdftotext failed to run\n";
        return "";
    }

    std::ifstream infile(txtPath);
    if (!infile.is_open()) {
        std::cerr << "[ERROR] Failed to open text output: " << txtPath << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    infile.close();

    return buffer.str();
}

bool DocumentRepository::loadDirectory(const std::string &path) {
    fs::path dirPath = fs::absolute(path);
    std::cout << "[DEBUG] Absolute document path: " << dirPath << "\n";

    if (!fs::exists(dirPath)) {
        std::cerr << "[ERROR] Document directory does not exist: " << dirPath << "\n";
        return false;
    }

    std::cout << "[DEBUG] Scanning directory: " << path << "\n";
    try {
        for (const auto &entry : fs::directory_iterator(path)) {
            std::cout << "[DEBUG] Found file: " << entry.path() << "\n";
            if (entry.path().extension() == ".pdf") {
                std::cout << "[DEBUG] Processing PDF: " << entry.path() << "\n";

                std::string pdfFile = entry.path().string();
                std::string text = runPdfToText(pdfFile);

                if (!text.empty()) {
                    docs.emplace_back(entry.path().filename().string(), text);
                    std::cout << "[DEBUG] Loaded: " << entry.path().filename() << "\n";
                } else {
                    std::cerr << "[ERROR] Empty text from: " << entry.path() << "\n";
                }
            } else {
                std::cout << "[DEBUG] Skipped (not PDF): " << entry.path() << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Exception reading directory: " << e.what() << "\n";
        return false;
    }

    std::cout << "[DEBUG] Total loaded documents: " << docs.size() << "\n";
    return true;
}


const std::vector<std::pair<std::string,std::string>>& 
DocumentRepository::getDocuments() const {
    return docs;
}
