#include "EmailProcessor.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = filesystem;
using namespace std;

EmailProcessor::EmailProcessor(const string& indexFilePath) {
    ReadIndexFile(indexFilePath);
}

//Reading Index Files
void EmailProcessor::ReadIndexFile(const string& filePath) {
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string key, value;
        if (getline(iss, key, ':') && getline(iss, value)) {
            value.erase(value.begin(), find_if(value.begin(), value.end(), [](unsigned char ch) {
                return !isspace(ch);
            }));
            emailMapping[key] = value;
        }
    }
}

//Skip Action
string EmailProcessor::SkipAction(const string& line) {
    return line;
}
//Reading per line
string EmailProcessor::ProcessLine(const string& line) {
    if (line.rfind("//", 0) == 0) {
        return SkipAction(line);
    }

    istringstream iss(line);
    string email, identifier;
    if (iss >> email >> identifier) {
        if (emailMapping.find(identifier) != emailMapping.end() && emailMapping[identifier] != "SKIP") {
            return email + emailMapping[identifier];
        } else if (identifier == "SKIP" || identifier == "DELETE" || identifier == "SAVE") {
            return SkipAction(line);
        }
    }
    return line + " (Error: Invalid format or unknown identifier)";
}

//Creating Single File
void EmailProcessor::ProcessFile(const string& inputPath, const string& outputPath) {
    if (!fs::exists(outputPath)) {
        ofstream createFile(outputPath);
    }

    ifstream inputFile(inputPath);
    ofstream outputFile(outputPath, ios::app);
    string line;
    while (getline(inputFile, line)) {
        string result = ProcessLine(line);
        if (!result.empty() && result.find("Error") == string::npos) {
            if (processedEmails.find(result) == processedEmails.end()) {
                outputFile << result << endl;
                processedEmails.insert(result);
            }
        } else if (!result.empty()) {
            outputFile << result << endl;
        }
    }
}
//Creating Directory of files
void EmailProcessor::ProcessFiles(const string& inputPrefix, const string& outputDir, const string& outputPrefix) {
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.path().filename().string().rfind(inputPrefix, 0) == 0) {
            string fileNumber = entry.path().filename().string().substr(inputPrefix.size());
            fileNumber = fileNumber.substr(0, fileNumber.find('.'));
            string outputPath = outputDir + "/" + outputPrefix + fileNumber + ".txt";
            ProcessFile(entry.path().string(), outputPath);
        }
    }
}
