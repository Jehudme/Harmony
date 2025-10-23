/**
 * InputExamples.cpp
 * 
 * This file contains example implementations demonstrating the InputManager system.
 * These examples show how to use keyboard, mouse, and virtual input in the Harmony Engine.
 */

#include "pch.h"
#include "Engine.h"
#include "InputManager.h"
#include "Script.h"
#include "Transform.h"
#include "Logger.h"

namespace Harmony::Examples
{
    // ===================================================================
    // Example 1: Player Movement Script
    // Demonstrates basic keyboard input for character movement
    // ===================================================================
    
    class PlayerMovementScript : public Components::Script
    {
    public:
        PlayerMovementScript(const Utilities::Configuration& config, Scenes::Scene& scene)
            : Script(config, scene), moveSpeed_(200.0f) {}

        void onPreUpdate() override
        {
            auto& input = getScene().engine.inputManager;
            auto& transform = getScene().componentReference<Components::Transform>(entityId);

            float deltaTime = getScene().engine.getDeltaTime();
            float x, y;
            transform.getPosition(x, y);

            // WASD movement
            if (input->isKeyHeld(Input::Key::W)) {
                y -= moveSpeed_ * deltaTime;
            }
            if (input->isKeyHeld(Input::Key::S)) {
                y += moveSpeed_ * deltaTime;
            }
            if (input->isKeyHeld(Input::Key::A)) {
                x -= moveSpeed_ * deltaTime;
            }
            if (input->isKeyHeld(Input::Key::D)) {
                x += moveSpeed_ * deltaTime;
            }

            // Sprint with Shift
            if (input->isKeyHeld(Input::Key::LShift)) {
                moveSpeed_ = 400.0f;
            }
            else {
                moveSpeed_ = 200.0f;
            }

            transform.setPosition(x, y);
        }

    private:
        float moveSpeed_;
    };

    // ===================================================================
    // Example 2: Action Input Script
    // Demonstrates one-time actions on key press
    // ===================================================================
    
    class ActionInputScript : public Components::Script
    {
    public:
        ActionInputScript(const Utilities::Configuration& config, Scenes::Scene& scene)
            : Script(config, scene), canJump_(true) {}

        void onPreUpdate() override
        {
            auto& input = getScene().engine.inputManager;

            // Jump on space bar - only once per press
            if (input->isKeyPressed(Input::Key::Space) && canJump_) {
                performJump();
            }

            // Attack on left mouse button
            if (input->isMouseButtonPressed(Input::MouseButton::Left)) {
                performAttack();
            }

            // Special ability on E key
            if (input->isKeyPressed(Input::Key::E)) {
                performSpecialAbility();
            }

            // Pause game on Escape
            if (input->isKeyPressed(Input::Key::Escape)) {
                pauseGame();
            }
        }

    private:
        bool canJump_;

        void performJump() {
            HARMONY_INFO("Player jumped!");
            canJump_ = false;
            // Jump logic here
        }

        void performAttack() {
            auto& input = getScene().engine.inputManager;
            auto [mouseX, mouseY] = input->getMousePosition();
            HARMONY_INFO("Attack towards mouse position ({}, {})", mouseX, mouseY);
            // Attack logic here
        }

        void performSpecialAbility() {
            HARMONY_INFO("Special ability activated!");
            // Special ability logic here
        }

        void pauseGame() {
            HARMONY_INFO("Game paused");
            // Pause logic here
        }
    };

    // ===================================================================
    // Example 3: Mouse Tracking Script
    // Demonstrates mouse position and button tracking
    // ===================================================================
    
    class MouseTrackingScript : public Components::Script
    {
    public:
        MouseTrackingScript(const Utilities::Configuration& config, Scenes::Scene& scene)
            : Script(config, scene) {}

        void onPreUpdate() override
        {
            auto& input = getScene().engine.inputManager;
            auto& transform = getScene().componentReference<Components::Transform>(entityId);

            // Make entity follow mouse cursor
            auto [mouseX, mouseY] = input->getMousePosition();
            transform.setPosition(static_cast<float>(mouseX), static_cast<float>(mouseY));

            // Rotate entity based on mouse wheel
            float wheelDelta = input->getMouseWheelDelta(Input::MouseWheel::Vertical);
            if (wheelDelta != 0.0f) {
                float currentRotation = transform.getRotation();
                transform.setRotation(currentRotation + wheelDelta * 10.0f);
            }

            // Change color/state on mouse button
            if (input->isMouseButtonHeld(Input::MouseButton::Left)) {
                HARMONY_TRACE("Left mouse button held at ({}, {})", mouseX, mouseY);
                // Visual feedback logic here
            }
        }
    };

    // ===================================================================
    // Example 4: Combo Input System
    // Demonstrates detecting key combinations
    // ===================================================================
    
    class ComboInputScript : public Components::Script
    {
    public:
        ComboInputScript(const Utilities::Configuration& config, Scenes::Scene& scene)
            : Script(config, scene) {}

        void onPreUpdate() override
        {
            auto& input = getScene().engine.inputManager;

            // Check for Ctrl+S to save
            if (input->isKeyHeld(Input::Key::LControl) && input->isKeyPressed(Input::Key::S)) {
                saveGame();
            }

            // Check for Alt+F4 to quit
            if (input->isKeyHeld(Input::Key::LAlt) && input->isKeyPressed(Input::Key::F4)) {
                quitGame();
            }

            // Check for Shift+Space for super jump
            if (input->isKeyHeld(Input::Key::LShift) && input->isKeyPressed(Input::Key::Space)) {
                performSuperJump();
            }
        }

    private:
        void saveGame() {
            HARMONY_INFO("Saving game...");
            // Save logic here
        }

        void quitGame() {
            HARMONY_INFO("Quitting game...");
            getScene().engine.stop();
        }

        void performSuperJump() {
            HARMONY_INFO("Super jump!");
            // Super jump logic here
        }
    };

    // ===================================================================
    // Example 5: Virtual Input for Testing
    // Demonstrates how to use virtual input for automated testing
    // ===================================================================
    
    void AutomatedInputTest(Engine& engine)
    {
        auto& input = engine.inputManager;

        HARMONY_INFO("Starting automated input test...");

        // Simulate movement sequence
        input->simulateKeyPress(Input::Key::W);
        HARMONY_INFO("Simulated W key press");

        // In a real test, you would wait for frames here
        // For demonstration purposes, we just log the actions

        input->simulateKeyRelease(Input::Key::W);
        HARMONY_INFO("Simulated W key release");

        // Simulate mouse interaction
        input->simulateMouseMove(320, 240);
        HARMONY_INFO("Simulated mouse move to center");

        input->simulateMouseButtonPress(Input::MouseButton::Left);
        HARMONY_INFO("Simulated left mouse button press");

        input->simulateMouseButtonRelease(Input::MouseButton::Left);
        HARMONY_INFO("Simulated left mouse button release");

        // Simulate mouse wheel
        input->simulateMouseWheelScroll(Input::MouseWheel::Vertical, 1.0f);
        HARMONY_INFO("Simulated mouse wheel scroll");

        HARMONY_INFO("Automated input test complete");
    }

    // ===================================================================
    // Example 6: Input State Management
    // Demonstrates clearing input states during state transitions
    // ===================================================================
    
    class StateTransitionScript : public Components::Script
    {
    public:
        StateTransitionScript(const Utilities::Configuration& config, Scenes::Scene& scene)
            : Script(config, scene) {}

        void onPreUpdate() override
        {
            auto& input = getScene().engine.inputManager;

            // Check for state transition trigger
            if (input->isKeyPressed(Input::Key::Enter)) {
                transitionToNewState();
            }
        }

    private:
        void transitionToNewState() {
            auto& engine = getScene().engine;
            
            HARMONY_INFO("Transitioning to new state...");

            // Clear all input states to prevent input carryover
            engine.inputManager->clearAllStates();

            // Now transition to the new state
            // auto task = std::make_unique<Tasks::SwitchStateTask>(newStateId);
            // engine.taskManagement->submit(std::move(task));
        }
    };

    // ===================================================================
    // Example 7: Debug Input Handler
    // Demonstrates using function keys for debug features
    // ===================================================================
    
    class DebugInputScript : public Components::Script
    {
    public:
        DebugInputScript(const Utilities::Configuration& config, Scenes::Scene& scene)
            : Script(config, scene), debugMode_(false) {}

        void onPreUpdate() override
        {
            auto& input = getScene().engine.inputManager;

            // Toggle debug mode with F1
            if (input->isKeyPressed(Input::Key::F1)) {
                debugMode_ = !debugMode_;
                HARMONY_INFO("Debug mode: {}", debugMode_ ? "ON" : "OFF");
            }

            // Reload resources with F5
            if (input->isKeyPressed(Input::Key::F5)) {
                HARMONY_INFO("Reloading resources...");
                // Resource reload logic
            }

            // Take screenshot with F12
            if (input->isKeyPressed(Input::Key::F12)) {
                takeScreenshot();
            }

            // Speed controls with Page Up/Down
            if (input->isKeyPressed(Input::Key::PageUp)) {
                increaseGameSpeed();
            }
            if (input->isKeyPressed(Input::Key::PageDown)) {
                decreaseGameSpeed();
            }
        }

    private:
        bool debugMode_;

        void takeScreenshot() {
            HARMONY_INFO("Screenshot captured");
            // Screenshot logic here
        }

        void increaseGameSpeed() {
            HARMONY_INFO("Game speed increased");
            // Speed modification logic
        }

        void decreaseGameSpeed() {
            HARMONY_INFO("Game speed decreased");
            // Speed modification logic
        }
    };

} // namespace Harmony::Examples

/*
 * Usage Notes:
 * 
 * 1. To use these example scripts, register them with HARMONY_REGISTER_SCRIPT:
 *    HARMONY_REGISTER_SCRIPT(PlayerMovementScript, "PlayerMovementScript");
 * 
 * 2. Then reference them in your configuration.json:
 *    "entities": {
 *        "1000": {
 *            "name": "Player",
 *            "script": "PlayerMovementScript",
 *            "components": { ... }
 *        }
 *    }
 * 
 * 3. For testing, call the AutomatedInputTest function:
 *    Harmony::Examples::AutomatedInputTest(engine);
 * 
 * 4. Key Points:
 *    - Use isKeyPressed/isMouseButtonPressed for one-time actions
 *    - Use isKeyHeld/isMouseButtonHeld for continuous actions
 *    - Clear input states when transitioning between game states
 *    - Virtual input is perfect for automated testing and replays
 */
