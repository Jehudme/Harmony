# Harmony Exception System

This document describes the exception hierarchy in the Harmony engine.

## Overview

The Harmony exception system provides a comprehensive hierarchy of exceptions organized into logical groups. All custom exceptions inherit from either `HarmonyException` (for runtime errors) or `HarmonyLogicError` (for programming mistakes/logic errors).

## Exception Hierarchy

```
std::exception
├── std::runtime_error
│   └── HarmonyException (Base for all runtime errors)
│       ├── ConfigurationException
│       │   ├── OpenConfigurationFileException
│       │   ├── ParseConfigurationFileException
│       │   ├── ConfigurationKeyNotFoundException
│       │   └── ConfigurationTypeException
│       ├── LoggerException
│       │   ├── LoggerInitializationException
│       │   └── LoggerFileException
│       └── FileSystemException
│           ├── FileOpenException
│           ├── FileReadException
│           ├── FileWriteException
│           └── InvalidFilePathException
│
└── std::logic_error
    └── HarmonyLogicError (Base for all logic errors)
        ├── ValidationException
        │   ├── OutOfRangeException
        │   ├── NullPointerException
        │   └── InvalidArgumentException
        ├── TimeException
        │   ├── InvalidTimeValueException
        │   └── InvalidTimerOperationException
        └── ColorException
            └── InvalidColorValueException
```

## Exception Groups

### Base Exceptions

- **HarmonyException**: Base class for all Harmony runtime errors. Automatically logs the error message.
- **HarmonyLogicError**: Base class for all Harmony logic errors (programming mistakes). Automatically logs the error message.

### Configuration Exceptions

Handle errors related to configuration file operations:

- **ConfigurationException**: Base exception for configuration-related errors
- **OpenConfigurationFileException**: Thrown when a configuration file cannot be opened
- **ParseConfigurationFileException**: Thrown when a configuration file cannot be parsed
- **ConfigurationKeyNotFoundException**: Thrown when a configuration key is not found
- **ConfigurationTypeException**: Thrown when a configuration value has an invalid type

**Example Usage:**
```cpp
try {
    config.load("settings.json");
} catch (const Harmony::Exceptions::OpenConfigurationFileException& e) {
    HARMONY_ERROR("Failed to load configuration: {}", e.what());
}
```

### Logger Exceptions

Handle errors related to logging system operations:

- **LoggerException**: Base exception for logger-related errors
- **LoggerInitializationException**: Thrown when logger initialization fails
- **LoggerFileException**: Thrown when logger file operations fail

**Example Usage:**
```cpp
try {
    Logger::initialize("app.log");
} catch (const Harmony::Exceptions::LoggerInitializationException& e) {
    std::cerr << "Logger initialization failed: " << e.what() << std::endl;
}
```

### File System Exceptions

Handle general file system operations:

- **FileSystemException**: Base exception for file system operations
- **FileOpenException**: Thrown when a file cannot be opened
- **FileReadException**: Thrown when a file read operation fails
- **FileWriteException**: Thrown when a file write operation fails
- **InvalidFilePathException**: Thrown when a file path is invalid

**Example Usage:**
```cpp
try {
    // File operations
} catch (const Harmony::Exceptions::FileReadException& e) {
    HARMONY_ERROR("Read error: {}", e.what());
}
```

### Validation Exceptions

Handle validation and argument checking errors:

- **ValidationException**: Base exception for validation errors
- **OutOfRangeException**: Thrown when a value is out of valid range (template-based)
- **NullPointerException**: Thrown when a null pointer is encountered where it shouldn't be
- **InvalidArgumentException**: Thrown when an invalid argument is provided

**Example Usage:**
```cpp
void setVolume(float volume) {
    if (volume < 0.0f || volume > 1.0f) {
        throw Harmony::Exceptions::OutOfRangeException("volume", volume, 0.0f, 1.0f);
    }
}
```

### Time Exceptions

Handle time-related errors:

- **TimeException**: Base exception for time-related errors
- **InvalidTimeValueException**: Thrown when a time value is invalid
- **InvalidTimerOperationException**: Thrown when a timer operation is invalid

**Example Usage:**
```cpp
void processTime(Time duration) {
    if (duration < Time::Zero) {
        throw Harmony::Exceptions::InvalidTimeValueException("Duration cannot be negative");
    }
}
```

### Color Exceptions

Handle color-related errors:

- **ColorException**: Base exception for color-related errors
- **InvalidColorValueException**: Thrown when a color value is invalid

**Example Usage:**
```cpp
void validateColorComponent(int value) {
    if (value < 0 || value > 255) {
        throw Harmony::Exceptions::InvalidColorValueException("red", value);
    }
}
```

## Features

### Automatic Logging

All exceptions automatically log their error messages using the Harmony logging system:
- Base exceptions log at ERROR level
- Some specific exceptions may use different log levels (e.g., WARN for non-critical errors)

### Detailed Error Messages

Exceptions provide detailed, formatted error messages that include:
- Context about what went wrong
- Relevant values (file paths, keys, values, ranges, etc.)
- Suggestions when applicable

### Template Support

The `OutOfRangeException` is templated to work with different numeric types:
```cpp
throw OutOfRangeException("index", 150, 0, 100);      // int
throw OutOfRangeException("volume", 1.5f, 0.0f, 1.0f); // float
```

## Best Practices

1. **Catch Specific Exceptions**: Always catch the most specific exception type when possible
2. **Use Appropriate Base Class**: Choose `HarmonyException` for runtime errors, `HarmonyLogicError` for programming mistakes
3. **Provide Context**: When throwing exceptions, provide detailed context in error messages
4. **Log Before Rethrowing**: If you catch and rethrow, consider logging additional context
5. **Don't Swallow Exceptions**: Always handle or propagate exceptions appropriately

## Integration with Assertions

The exception system works alongside the assertion system:
- Use **assertions** for checking preconditions and invariants in debug builds
- Use **exceptions** for recoverable runtime errors that can occur in production

Example:
```cpp
void processData(const Data* data, int index) {
    // Assert checks programming errors (debug only)
    HARMONY_ASSERT_NOT_NULL(data, "Data pointer is null");
    HARMONY_ASSERT_RANGE(index, 0, data->size() - 1, "Index out of bounds");
    
    // Exceptions handle runtime errors (always active)
    if (index < 0 || index >= data->size()) {
        throw Harmony::Exceptions::OutOfRangeException("index", index, 0, data->size() - 1);
    }
}
```
