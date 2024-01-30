#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std; 

class EmailProcessor {
public:
    EmailProcessor(const string& indexFilePath);
    void ProcessFiles(const string& inputPrefix, const string& outputDir, const string& outputPrefix);

private:
    unordered_map<string, string> emailMapping;
    unordered_set<string> processedEmails;

    void ReadIndexFile(const string& filePath);
    string ProcessLine(const string& line);
    void ProcessFile(const string& inputPath, const string& outputPath);
    string SkipAction(const string& line);
};
