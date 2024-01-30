#include "EmailProcessor.h"

int main() {
    std::string indexFilePath = "index.txt";
    EmailProcessor processor(indexFilePath);

    std::string inputPrefix = "list";
    std::string outputDir = "emails";
    std::string outputPrefix = "output";

    processor.ProcessFiles(inputPrefix, outputDir, outputPrefix);

    return 0;
}
