#include <Harmony/Exceptions.h>
#include <Harmony/Logger.h>
#include <iostream>

// Example demonstrating the new exception system
// This file is for documentation and testing purposes

void demonstrateConfigurationExceptions() {
    using namespace Harmony::Utilities;
    using namespace Harmony::Exceptions;
    
    Logger::initialize("ExceptionDemo.log");
    
    std::cout << "=== Configuration Exceptions Demo ===" << std::endl;
    
    try {
        throw OpenConfigurationFileException("config.json");
    } catch (const ConfigurationException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw ParseConfigurationFileException("settings.json", "Invalid JSON format");
    } catch (const ConfigurationException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw ConfigurationKeyNotFoundException("database.host");
    } catch (const ConfigurationException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw ConfigurationTypeException("port", "integer", "string");
    } catch (const ConfigurationException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
}

void demonstrateValidationExceptions() {
    using namespace Harmony::Exceptions;
    
    std::cout << "\n=== Validation Exceptions Demo ===" << std::endl;
    
    try {
        throw OutOfRangeException("volume", 1.5f, 0.0f, 1.0f);
    } catch (const ValidationException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw NullPointerException("dataPointer");
    } catch (const ValidationException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw InvalidArgumentException("username", "cannot be empty");
    } catch (const ValidationException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
}

void demonstrateTimeExceptions() {
    using namespace Harmony::Exceptions;
    
    std::cout << "\n=== Time Exceptions Demo ===" << std::endl;
    
    try {
        throw InvalidTimeValueException("negative duration provided");
    } catch (const TimeException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw InvalidTimerOperationException("start", "timer is already running");
    } catch (const TimeException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
}

void demonstrateFileSystemExceptions() {
    using namespace Harmony::Exceptions;
    
    std::cout << "\n=== File System Exceptions Demo ===" << std::endl;
    
    try {
        throw FileOpenException("/path/to/file.txt", "read");
    } catch (const FileSystemException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw FileReadException("data.bin", "unexpected end of file");
    } catch (const FileSystemException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw FileWriteException("output.txt", "disk full");
    } catch (const FileSystemException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw InvalidFilePathException("../../../etc/passwd", "path traversal not allowed");
    } catch (const FileSystemException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
}

void demonstrateColorExceptions() {
    using namespace Harmony::Exceptions;
    
    std::cout << "\n=== Color Exceptions Demo ===" << std::endl;
    
    try {
        throw InvalidColorValueException("red", 300);
    } catch (const ColorException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
}

void demonstrateBaseExceptions() {
    using namespace Harmony::Exceptions;
    
    std::cout << "\n=== Base Exceptions Demo ===" << std::endl;
    
    try {
        throw HarmonyException("Generic runtime error");
    } catch (const HarmonyException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    
    try {
        throw HarmonyLogicError("Programming logic error");
    } catch (const HarmonyLogicError& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
}

int main() {
    try {
        demonstrateConfigurationExceptions();
        demonstrateValidationExceptions();
        demonstrateTimeExceptions();
        demonstrateFileSystemExceptions();
        demonstrateColorExceptions();
        demonstrateBaseExceptions();
        
        std::cout << "\n=== All exception demos completed successfully ===" << std::endl;
        
        Harmony::Utilities::Logger::shutdown();
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
