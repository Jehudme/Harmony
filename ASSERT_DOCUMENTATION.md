# Assert Macros Documentation

The Harmony engine provides a comprehensive set of assertion macros for runtime validation and debugging. These macros integrate with the logging system to provide detailed error reporting.

## Features

- **Multiple assertion levels**: Standard, warning, critical, and debug-only
- **Automatic logging**: All failed assertions are logged with file and line information
- **Optional messages**: Add custom error messages to provide context
- **Build-aware behavior**: Different behavior in debug vs release builds
- **Convenience macros**: Common checks like null pointers and range validation

## Basic Usage

### Standard Assertion
```cpp
#include <Harmony/Assert.h>

// Assert with message
HARMONY_ASSERT(value > 0, "Value must be positive");

// Assert without message
HARMONY_ASSERT(ptr != nullptr);
```

**Behavior:**
- **Debug builds**: Logs error and aborts on failure
- **Release builds**: Logs error but continues execution

### Warning Assertion
```cpp
// Logs a warning but never aborts
HARMONY_ASSERT_WARN(count < 1000, "Count is getting high");
```

**Behavior:**
- Logs a warning on failure
- **Never aborts**, even in debug builds
- Useful for non-critical checks

### Critical Assertion
```cpp
// Always aborts on failure, even in release builds
HARMONY_ASSERT_CRITICAL(initialized, "System must be initialized");
```

**Behavior:**
- Logs a critical error on failure
- **Always aborts** in both debug and release builds
- Use for unrecoverable errors

### Debug-Only Assertion
```cpp
// Completely removed in release builds
HARMONY_ASSERT_DEBUG(internalStateValid, "Internal state check");
```

**Behavior:**
- **Debug builds**: Logs error and aborts on failure
- **Release builds**: Completely removed (zero overhead)
- Ideal for expensive validation checks

## Convenience Macros

### Not Null Check
```cpp
HARMONY_ASSERT_NOT_NULL(ptr, "Pointer should not be null");
```
Equivalent to: `HARMONY_ASSERT(ptr != nullptr, "...")`

### Range Check
```cpp
HARMONY_ASSERT_RANGE(value, 0, 100, "Value out of range");
```
Equivalent to: `HARMONY_ASSERT(value >= 0 && value <= 100, "...")`

### Equality Check
```cpp
HARMONY_ASSERT_EQUAL(actual, expected, "Values don't match");
```
Equivalent to: `HARMONY_ASSERT(actual == expected, "...")`

### Not Equal Check
```cpp
HARMONY_ASSERT_NOT_EQUAL(a, b, "Values should differ");
```
Equivalent to: `HARMONY_ASSERT(a != b, "...")`

## Log Output Format

When an assertion fails, it logs in the following format:
```
[ERROR] Assertion failed: <condition> | <message> | File: <file> | Line: <line>
```

Example:
```
[ERROR] Assertion failed: value > 0 | Value must be positive | File: main.cpp | Line: 42
```

## Best Practices

1. **Use appropriate assertion levels**:
   - `HARMONY_ASSERT`: General validation
   - `HARMONY_ASSERT_WARN`: Non-critical issues
   - `HARMONY_ASSERT_CRITICAL`: Unrecoverable errors
   - `HARMONY_ASSERT_DEBUG`: Expensive checks

2. **Provide meaningful messages**:
   ```cpp
   // Good
   HARMONY_ASSERT(entity != nullptr, "Entity pointer was unexpectedly null");
   
   // Less helpful
   HARMONY_ASSERT(entity != nullptr);
   ```

3. **Don't use assertions for expected errors**:
   - Assertions are for programming errors (bugs)
   - Use exceptions or error codes for runtime errors (user input, file I/O, etc.)

4. **Keep assertions side-effect free**:
   ```cpp
   // Bad - counter gets incremented even if assertion is removed
   HARMONY_ASSERT(++counter < 10);
   
   // Good
   counter++;
   HARMONY_ASSERT(counter < 10);
   ```

5. **Use debug assertions for expensive checks**:
   ```cpp
   // Expensive validation only in debug builds
   HARMONY_ASSERT_DEBUG(validateDataStructureIntegrity(), "Data structure corrupted");
   ```

## Integration with Logger

The assert macros use the Harmony Logger system. Make sure to initialize the logger before using assertions:

```cpp
#include <Harmony/Logger.h>
#include <Harmony/Assert.h>

int main() {
    Harmony::Utilities::Logger::initialize("MyApp.log");
    
    // Now assertions will log properly
    HARMONY_ASSERT(true, "This passes");
    
    Harmony::Utilities::Logger::shutdown();
    return 0;
}
```

## Comparison with Standard assert()

| Feature | Standard assert() | HARMONY_ASSERT |
|---------|------------------|----------------|
| Logging | No | Yes (with file/line) |
| Custom messages | No | Yes |
| Warning level | No | Yes (HARMONY_ASSERT_WARN) |
| Critical level | No | Yes (HARMONY_ASSERT_CRITICAL) |
| Release behavior | Removed | Logs but may continue |
| Multiple levels | No | Yes (4 levels) |

## Examples

See `Sandbox/AssertDemo.cpp` for complete working examples of all assertion macros.
