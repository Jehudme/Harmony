#include <Harmony/Assert.h>
#include <Harmony/Logger.h>

// Example demonstrating the assert macro usage
// This file is for documentation purposes only and is not compiled by default

void demonstrateAssertUsage() {
    using namespace Harmony::Utilities;
    
    // Initialize logger first
    Logger::initialize("AssertDemo.log");
    
    // Example 1: Basic assertion that passes
    int value = 42;
    HARMONY_ASSERT(value == 42, "Value should be 42");
    
    // Example 2: Assertion with no message
    HARMONY_ASSERT(value > 0);
    
    // Example 3: Warning assertion (logs but never aborts)
    HARMONY_ASSERT_WARN(value < 100, "Value is getting large");
    
    // Example 4: Not null assertion
    int* ptr = &value;
    HARMONY_ASSERT_NOT_NULL(ptr, "Pointer should not be null");
    
    // Example 5: Range assertion
    HARMONY_ASSERT_RANGE(value, 0, 100, "Value should be in range [0, 100]");
    
    // Example 6: Equality assertion
    int expected = 42;
    HARMONY_ASSERT_EQUAL(value, expected, "Values should match");
    
    // Example 7: Debug-only assertion (only active in debug builds)
    HARMONY_ASSERT_DEBUG(value > 0, "This check only happens in debug mode");
    
    // Example 8: Critical assertion (always aborts even in release)
    // HARMONY_ASSERT_CRITICAL(false, "This would abort the program");
    
    Logger::shutdown();
}

// Example of a failing assertion (commented out to prevent abort)
void demonstrateFailingAssertion() {
    using namespace Harmony::Utilities;
    
    Logger::initialize("AssertDemo.log");
    
    // This would log an error and abort in debug builds:
    // HARMONY_ASSERT(false, "This assertion will fail");
    
    // This would log a warning but continue execution:
    HARMONY_ASSERT_WARN(false, "This is a warning, program continues");
    
    // This would log a critical error and abort in all builds:
    // HARMONY_ASSERT_CRITICAL(false, "Critical failure - program will abort");
    
    Logger::shutdown();
}
