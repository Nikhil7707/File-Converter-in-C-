#include "../include/FileConverter.h"
#include <iostream>
#include <string>

void printUsage() {
    std::cout << "Usage: FileConverter <input_file> <output_file>" << std::endl;
    std::cout << "Supported formats: TXT, CSV, JSON, XML" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage();
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    converter::FileConverter converter;
    
    // Print supported formats
    std::cout << "Supported formats:" << std::endl;
    for (auto format : converter.getSupportedFormats()) {
        std::cout << "- " << converter::FileConverter::getExtension(format) << std::endl;
    }
    
    // Perform conversion
    std::cout << "Converting " << inputFile << " to " << outputFile << "..." << std::endl;
    
    if (converter.convert(inputFile, outputFile)) {
        std::cout << "Conversion successful!" << std::endl;
        return 0;
    } else {
        std::cerr << "Conversion failed!" << std::endl;
        return 1;
    }
}