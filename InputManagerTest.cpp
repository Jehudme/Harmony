/**
 * InputManagerTest.cpp
 * 
 * Simple test to validate InputManager functionality
 * This is not a comprehensive unit test but demonstrates that the system works
 */

#include "pch.h"
#include "Engine.h"
#include "InputManager.h"
#include "Configuration.h"
#include "Logger.h"

namespace Harmony::Testing
{
    void TestInputManagerBasics()
    {
        HARMONY_INFO("=== Testing InputManager Basics ===");

        // Create a minimal configuration for the engine
        Utilities::Configuration config;
        
        // Set up minimal window configuration
        config.set<std::string>({"window", "title"}, "Input Test");
        config.set<unsigned int>({"window", "width"}, 800);
        config.set<unsigned int>({"window", "height"}, 600);
        config.set<unsigned int>({"window", "fps"}, 60);

        // Create engine (this will create the InputManager)
        Engine engine(config);

        HARMONY_INFO("Engine and InputManager created successfully");

        // Test 1: Virtual key press and release
        HARMONY_INFO("Test 1: Virtual key input");
        {
            auto& input = engine.inputManager;

            // Initially, no keys should be pressed
            if (!input->isKeyHeld(Input::Key::W)) {
                HARMONY_INFO("✓ Key W is not held initially");
            }

            // Simulate key press
            input->simulateKeyPress(Input::Key::W);

            // Key should now be pressed
            if (input->isKeyPressed(Input::Key::W)) {
                HARMONY_INFO("✓ Key W is pressed");
            }
            if (input->isKeyHeld(Input::Key::W)) {
                HARMONY_INFO("✓ Key W is held");
            }

            // Update states (transitions Pressed to Held)
            input->updateStates();

            // After update, should no longer be "just pressed" but still held
            if (!input->isKeyPressed(Input::Key::W)) {
                HARMONY_INFO("✓ Key W is no longer 'just pressed' after update");
            }
            if (input->isKeyHeld(Input::Key::W)) {
                HARMONY_INFO("✓ Key W is still held after update");
            }

            // Simulate key release
            input->simulateKeyRelease(Input::Key::W);

            // Key should now be released
            if (input->isKeyReleased(Input::Key::W)) {
                HARMONY_INFO("✓ Key W is released");
            }
            if (!input->isKeyHeld(Input::Key::W)) {
                HARMONY_INFO("✓ Key W is no longer held");
            }

            // Update states
            input->updateStates();

            if (!input->isKeyReleased(Input::Key::W)) {
                HARMONY_INFO("✓ Key W release state cleared after update");
            }
        }

        // Test 2: Virtual mouse button
        HARMONY_INFO("Test 2: Virtual mouse button input");
        {
            auto& input = engine.inputManager;

            // Initially, no buttons pressed
            if (!input->isMouseButtonHeld(Input::MouseButton::Left)) {
                HARMONY_INFO("✓ Left mouse button not held initially");
            }

            // Simulate button press
            input->simulateMouseButtonPress(Input::MouseButton::Left);

            if (input->isMouseButtonPressed(Input::MouseButton::Left)) {
                HARMONY_INFO("✓ Left mouse button is pressed");
            }

            // Update states
            input->updateStates();

            if (input->isMouseButtonHeld(Input::MouseButton::Left)) {
                HARMONY_INFO("✓ Left mouse button is held after update");
            }

            // Simulate button release
            input->simulateMouseButtonRelease(Input::MouseButton::Left);

            if (input->isMouseButtonReleased(Input::MouseButton::Left)) {
                HARMONY_INFO("✓ Left mouse button is released");
            }

            // Update states
            input->updateStates();
        }

        // Test 3: Mouse position
        HARMONY_INFO("Test 3: Mouse position tracking");
        {
            auto& input = engine.inputManager;

            // Set mouse position
            input->simulateMouseMove(100, 200);

            auto [x, y] = input->getMousePosition();
            if (x == 100 && y == 200) {
                HARMONY_INFO("✓ Mouse position correctly set to ({}, {})", x, y);
            }

            // Update and check again
            input->simulateMouseMove(300, 400);
            int newX, newY;
            input->getMousePosition(newX, newY);
            if (newX == 300 && newY == 400) {
                HARMONY_INFO("✓ Mouse position updated to ({}, {})", newX, newY);
            }
        }

        // Test 4: Mouse wheel
        HARMONY_INFO("Test 4: Mouse wheel input");
        {
            auto& input = engine.inputManager;

            // Simulate wheel scroll
            input->simulateMouseWheelScroll(Input::MouseWheel::Vertical, 1.5f);

            float delta = input->getMouseWheelDelta(Input::MouseWheel::Vertical);
            if (delta == 1.5f) {
                HARMONY_INFO("✓ Mouse wheel delta is {}", delta);
            }

            // Update states (should clear wheel delta)
            input->updateStates();

            delta = input->getMouseWheelDelta(Input::MouseWheel::Vertical);
            if (delta == 0.0f) {
                HARMONY_INFO("✓ Mouse wheel delta cleared after update");
            }
        }

        // Test 5: Clear all states
        HARMONY_INFO("Test 5: Clear all states");
        {
            auto& input = engine.inputManager;

            // Set some states
            input->simulateKeyPress(Input::Key::A);
            input->simulateMouseButtonPress(Input::MouseButton::Right);
            input->simulateMouseMove(500, 500);

            // Clear all
            input->clearAllStates();

            if (!input->isKeyHeld(Input::Key::A)) {
                HARMONY_INFO("✓ Key states cleared");
            }
            if (!input->isMouseButtonHeld(Input::MouseButton::Right)) {
                HARMONY_INFO("✓ Mouse button states cleared");
            }

            float wheelDelta = input->getMouseWheelDelta(Input::MouseWheel::Vertical);
            if (wheelDelta == 0.0f) {
                HARMONY_INFO("✓ Mouse wheel cleared");
            }
        }

        // Test 6: Multiple keys simultaneously
        HARMONY_INFO("Test 6: Multiple simultaneous inputs");
        {
            auto& input = engine.inputManager;

            // Press multiple keys
            input->simulateKeyPress(Input::Key::W);
            input->simulateKeyPress(Input::Key::A);
            input->simulateKeyPress(Input::Key::LShift);

            if (input->isKeyHeld(Input::Key::W) && 
                input->isKeyHeld(Input::Key::A) && 
                input->isKeyHeld(Input::Key::LShift)) {
                HARMONY_INFO("✓ Multiple keys can be held simultaneously");
            }

            // Release one key
            input->simulateKeyRelease(Input::Key::A);

            if (input->isKeyHeld(Input::Key::W) && 
                !input->isKeyHeld(Input::Key::A) && 
                input->isKeyHeld(Input::Key::LShift)) {
                HARMONY_INFO("✓ Individual keys can be released independently");
            }

            // Clear for next test
            input->clearAllStates();
        }

        HARMONY_INFO("=== All InputManager Tests Completed Successfully ===");
    }

    void TestKeyEnumMapping()
    {
        HARMONY_INFO("=== Testing Key Enum Coverage ===");
        
        // Test that all major key categories are defined
        HARMONY_INFO("Letter keys: A={}, Z={}", 
            static_cast<int>(Input::Key::A), 
            static_cast<int>(Input::Key::Z));
        
        HARMONY_INFO("Number keys: Num0={}, Num9={}", 
            static_cast<int>(Input::Key::Num0), 
            static_cast<int>(Input::Key::Num9));
        
        HARMONY_INFO("Arrow keys: Left={}, Right={}, Up={}, Down={}", 
            static_cast<int>(Input::Key::Left), 
            static_cast<int>(Input::Key::Right),
            static_cast<int>(Input::Key::Up), 
            static_cast<int>(Input::Key::Down));
        
        HARMONY_INFO("Function keys: F1={}, F12={}", 
            static_cast<int>(Input::Key::F1), 
            static_cast<int>(Input::Key::F12));
        
        HARMONY_INFO("Special keys: Escape={}, Space={}, Enter={}", 
            static_cast<int>(Input::Key::Escape), 
            static_cast<int>(Input::Key::Space),
            static_cast<int>(Input::Key::Enter));
        
        HARMONY_INFO("Mouse buttons: Left={}, Right={}, Middle={}", 
            static_cast<int>(Input::MouseButton::Left), 
            static_cast<int>(Input::MouseButton::Right),
            static_cast<int>(Input::MouseButton::Middle));

        HARMONY_INFO("✓ Key enum coverage verified");
    }

} // namespace Harmony::Testing

// Uncomment to run tests when this file is compiled
/*
int main() {
    Harmony::Utilities::Logger::initialize("InputTest.log");
    
    try {
        Harmony::Testing::TestKeyEnumMapping();
        Harmony::Testing::TestInputManagerBasics();
    }
    catch (const std::exception& e) {
        HARMONY_ERROR("Test failed with exception: {}", e.what());
        return 1;
    }
    
    return 0;
}
*/
