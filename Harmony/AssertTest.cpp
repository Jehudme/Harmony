// Integration test for Assert.h
// This file demonstrates that Assert.h integrates properly with the Harmony engine

#include "pch.h"
#include "Assert.h"
#include "Logger.h"

namespace Harmony::Tests {

// Test function demonstrating assertion usage in various scenarios
void testAssertMacros() {
    // Test 1: Passing assertion (should not log anything)
    int validValue = 50;
    HARMONY_ASSERT(validValue > 0, "Value should be positive");
    
    // Test 2: Null pointer check
    int* validPtr = &validValue;
    HARMONY_ASSERT_NOT_NULL(validPtr, "Pointer should be valid");
    
    // Test 3: Range validation
    HARMONY_ASSERT_RANGE(validValue, 0, 100, "Value should be in valid range");
    
    // Test 4: Equality check
    int expected = 50;
    HARMONY_ASSERT_EQUAL(validValue, expected, "Values should match");
    
    // Test 5: Not equal check
    int different = 99;
    HARMONY_ASSERT_NOT_EQUAL(validValue, different, "Values should be different");
    
    // Test 6: Debug-only assertion
    HARMONY_ASSERT_DEBUG(validValue == 50, "Debug validation passed");
    
    // Test 7: Warning assertion (logs but doesn't abort)
    HARMONY_ASSERT_WARN(validValue < 1000, "Value is within acceptable limits");
}

// Example of assertion usage in a class
class AssertionExample {
public:
    AssertionExample(int* data, size_t size) : data_(data), size_(size) {
        // Critical assertions in constructor
        HARMONY_ASSERT_NOT_NULL(data, "Data pointer must not be null in constructor");
        HARMONY_ASSERT(size > 0, "Size must be positive");
    }
    
    void processData() {
        // Assert preconditions
        HARMONY_ASSERT_NOT_NULL(data_, "Data should be initialized");
        HARMONY_ASSERT_RANGE(size_, 1, 10000, "Size should be reasonable");
        
        // Process data...
        for (size_t i = 0; i < size_; ++i) {
            HARMONY_ASSERT_DEBUG(i < size_, "Index should be in bounds");
            // Process data_[i]
        }
    }
    
private:
    int* data_;
    size_t size_;
};

} // namespace Harmony::Tests

// Example usage in component code
namespace Harmony::Components {

class ComponentWithAssertions {
public:
    void initialize() {
        // Critical assertion - must be initialized only once
        HARMONY_ASSERT(!initialized_, "Component should not be initialized twice");
        
        initialized_ = true;
    }
    
    void update(float deltaTime) {
        // Warn if delta time is unusual
        HARMONY_ASSERT_WARN(deltaTime > 0.0f && deltaTime < 1.0f, 
            "Delta time seems unusual");
        
        // Assert we're initialized
        HARMONY_ASSERT(initialized_, "Component must be initialized before update");
    }
    
private:
    bool initialized_ = false;
};

} // namespace Harmony::Components
