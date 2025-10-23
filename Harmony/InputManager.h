#pragma once

#include <memory>
#include <array>
#include <unordered_map>

// Forward declarations - no external library headers
namespace sf {
	class Event;
}

namespace Harmony
{
	class Engine;

	namespace Input
	{
		/// @brief Custom keyboard key codes for input system
		enum class Key
		{
			Unknown = -1,
			A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
			Escape, LControl, LShift, LAlt, LSystem,
			RControl, RShift, RAlt, RSystem,
			Menu, LBracket, RBracket, Semicolon, Comma, Period, Quote, Slash, Backslash,
			Tilde, Equal, Hyphen, Space, Enter, Backspace, Tab,
			PageUp, PageDown, End, Home, Insert, Delete,
			Add, Subtract, Multiply, Divide,
			Left, Right, Up, Down,
			Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
			Pause,
			KeyCount // Keep last - used for array size
		};

		/// @brief Custom mouse button codes for input system
		enum class MouseButton
		{
			Left = 0,
			Right,
			Middle,
			XButton1,
			XButton2,
			ButtonCount // Keep last - used for array size
		};

		/// @brief Mouse wheel direction
		enum class MouseWheel
		{
			Vertical,
			Horizontal
		};

		/// @brief Input state for key/button tracking
		enum class InputState
		{
			Released,	// Not currently pressed
			Pressed,	// Just pressed this frame
			Held		// Held down from previous frame
		};
	}

	/// @brief Manages all input state for keyboard, mouse, and provides virtual input injection
	/// This class stores and tracks the state of every keyboard key, mouse button,
	/// and mouse position. It processes events from SFML and allows simulation of
	/// virtual input events that the program treats as real input.
	class InputManager
	{
	public:
		explicit InputManager(Engine& engine);
		~InputManager();

		// Prevent copying
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

		/// @brief Process an SFML event to update input states
		/// This method should be called for every sf::Event polled from the window
		/// @param event The SFML event to process
		void handleEvent(const sf::Event& event);

		/// @brief Update input states at the end of the frame
		/// This should be called once per frame to transition "Pressed" states to "Held"
		/// and "Released" states that occurred this frame back to stable states
		void updateStates();

		// ===== Keyboard Query Functions =====

		/// @brief Check if a key is currently pressed (this frame only)
		/// @param key The key to check
		/// @return true if the key was just pressed this frame
		bool isKeyPressed(Input::Key key) const;

		/// @brief Check if a key is currently released (this frame only)
		/// @param key The key to check
		/// @return true if the key was just released this frame
		bool isKeyReleased(Input::Key key) const;

		/// @brief Check if a key is currently held down
		/// @param key The key to check
		/// @return true if the key is held down (pressed or held state)
		bool isKeyHeld(Input::Key key) const;

		// ===== Mouse Query Functions =====

		/// @brief Check if a mouse button is currently pressed (this frame only)
		/// @param button The mouse button to check
		/// @return true if the button was just pressed this frame
		bool isMouseButtonPressed(Input::MouseButton button) const;

		/// @brief Check if a mouse button is currently released (this frame only)
		/// @param button The mouse button to check
		/// @return true if the button was just released this frame
		bool isMouseButtonReleased(Input::MouseButton button) const;

		/// @brief Check if a mouse button is currently held down
		/// @param button The mouse button to check
		/// @return true if the button is held down (pressed or held state)
		bool isMouseButtonHeld(Input::MouseButton button) const;

		/// @brief Get the current mouse position in window coordinates
		/// @param x Output parameter for X coordinate
		/// @param y Output parameter for Y coordinate
		void getMousePosition(int& x, int& y) const;

		/// @brief Get the current mouse position in window coordinates
		/// @return Pair of (x, y) coordinates
		std::pair<int, int> getMousePosition() const;

		/// @brief Get mouse wheel delta for this frame
		/// @param wheel Which wheel to query (vertical or horizontal)
		/// @return The scroll delta for this frame
		float getMouseWheelDelta(Input::MouseWheel wheel) const;

		// ===== Virtual Input Functions =====
		// These methods allow simulating input events that the system treats as real input

		/// @brief Simulate a key press event
		/// This acts as if the user pressed the key this frame
		/// @param key The key to simulate pressing
		void simulateKeyPress(Input::Key key);

		/// @brief Simulate a key release event
		/// This acts as if the user released the key this frame
		/// @param key The key to simulate releasing
		void simulateKeyRelease(Input::Key key);

		/// @brief Simulate a mouse button press event
		/// @param button The mouse button to simulate pressing
		void simulateMouseButtonPress(Input::MouseButton button);

		/// @brief Simulate a mouse button release event
		/// @param button The mouse button to simulate releasing
		void simulateMouseButtonRelease(Input::MouseButton button);

		/// @brief Simulate mouse movement
		/// @param x New X position in window coordinates
		/// @param y New Y position in window coordinates
		void simulateMouseMove(int x, int y);

		/// @brief Simulate mouse wheel scroll
		/// @param wheel Which wheel to scroll
		/// @param delta Amount to scroll
		void simulateMouseWheelScroll(Input::MouseWheel wheel, float delta);

		/// @brief Clear all input states (useful for state transitions)
		void clearAllStates();

	private:
		// Helper methods for state management
		void setKeyState(Input::Key key, Input::InputState state);
		void setMouseButtonState(Input::MouseButton button, Input::InputState state);
		Input::InputState getKeyState(Input::Key key) const;
		Input::InputState getMouseButtonState(Input::MouseButton button) const;

		// Convert SFML key codes to our custom key codes
		Input::Key sfmlKeyToCustomKey(int sfmlKey) const;
		Input::MouseButton sfmlButtonToCustomButton(int sfmlButton) const;

	private:
		Engine& engine_;

		// Input state storage
		std::array<Input::InputState, static_cast<size_t>(Input::Key::KeyCount)> keyStates_;
		std::array<Input::InputState, static_cast<size_t>(Input::MouseButton::ButtonCount)> mouseButtonStates_;
		
		// Keys that were just pressed or released this frame (for state transition)
		std::array<bool, static_cast<size_t>(Input::Key::KeyCount)> keysJustPressed_;
		std::array<bool, static_cast<size_t>(Input::Key::KeyCount)> keysJustReleased_;
		std::array<bool, static_cast<size_t>(Input::MouseButton::ButtonCount)> buttonsJustPressed_;
		std::array<bool, static_cast<size_t>(Input::MouseButton::ButtonCount)> buttonsJustReleased_;

		// Mouse position and wheel state
		int mouseX_;
		int mouseY_;
		float mouseWheelVerticalDelta_;
		float mouseWheelHorizontalDelta_;
	};

} // namespace Harmony
