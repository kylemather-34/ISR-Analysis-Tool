#ifndef DOCUMENTREPOSITORY_HPP
#define DOCUMENTREPOSITORY_HPP

#include <string>
#include <vector>
#include <utility>

class DocumentRepository {
public:
    // Load all PDF files in a directory and extract their text
    bool loadDirectory(const std::string &path);

    // Return vector of {filename, extracted_text}
    const std::vector<std::pair<std::string, std::string>>& getDocuments() const;

private:
    // Stored documents: pair = {filename, text_content}
    std::vector<std::pair<std::string, std::string>> docs;
};

#endif // DOCUMENTREPOSITORY_HPP
