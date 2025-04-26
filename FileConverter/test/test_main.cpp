#include "../include/FileConverter.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>

// Simple test function
void testFormatDetection() {
    converter::FileConverter converter;
    
    assert(converter::FileConverter::detectFormat("test.txt") == converter::FileFormat::TXT);
    assert(converter::FileConverter::detectFormat("test.csv") == converter::FileFormat::CSV);
    assert(converter::FileConverter::detectFormat("test.json") == converter::FileFormat::JSON);
    assert(converter::FileConverter::detectFormat("test.xml") == converter::FileFormat::XML);
    assert(converter::FileConverter::detectFormat("test.unknown") == converter::FileFormat::UNKNOWN);
    
    std::cout << "Format detection test passed!" << std::endl;
}

// Test conversion
void testConversion() {
    // Create test files
    {
        std::ofstream testFile("test_input.txt");
        testFile << "Hello World Test File" << std::endl;
        testFile << "This is a test" << std::endl;
    }
    
    converter::FileConverter converter;
    bool result = converter.convert("test_input.txt", "test_output.csv");
    assert(result);
    
    // Verify conversion
    std::ifstream output("test_output.csv");
    std::string line;
    std::getline(output, line);
    assert(line == "Hello,World,Test,File");
    
    std::cout << "Conversion test passed!" << std::endl;
    
    // Clean up
    std::remove("test_input.txt");
    std::remove("test_output.csv");
}

int main() {
    testFormatDetection();
    testConversion();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}