#include "pch.h"

#include <SFML/Window.hpp>

#include "InputManager.h"
#include "Engine.h"
#include "Logger.h"

namespace Harmony
{
	InputManager::InputManager(Engine& engine)
		: engine_(engine)
		, mouseX_(0)
		, mouseY_(0)
		, mouseWheelVerticalDelta_(0.0f)
		, mouseWheelHorizontalDelta_(0.0f)
	{
		// Initialize all input states to Released
		keyStates_.fill(Input::InputState::Released);
		mouseButtonStates_.fill(Input::InputState::Released);
		keysJustPressed_.fill(false);
		keysJustReleased_.fill(false);
		buttonsJustPressed_.fill(false);
		buttonsJustReleased_.fill(false);

		HARMONY_INFO("InputManager initialized");
	}

	InputManager::~InputManager()
	{
		HARMONY_INFO("InputManager shutting down");
	}

	void InputManager::handleEvent(const sf::Event& event)
	{
		// Process the event based on its type
		switch (event.type)
		{
		case sf::Event::KeyPressed:
		{
			Input::Key key = sfmlKeyToCustomKey(event.key.code);
			if (key != Input::Key::Unknown)
			{
				// Only mark as pressed if it wasn't already held
				if (keyStates_[static_cast<size_t>(key)] == Input::InputState::Released)
				{
					keyStates_[static_cast<size_t>(key)] = Input::InputState::Pressed;
					keysJustPressed_[static_cast<size_t>(key)] = true;
					HARMONY_TRACE("Key pressed: {}", static_cast<int>(key));
				}
			}
			break;
		}

		case sf::Event::KeyReleased:
		{
			Input::Key key = sfmlKeyToCustomKey(event.key.code);
			if (key != Input::Key::Unknown)
			{
				keyStates_[static_cast<size_t>(key)] = Input::InputState::Released;
				keysJustReleased_[static_cast<size_t>(key)] = true;
				HARMONY_TRACE("Key released: {}", static_cast<int>(key));
			}
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			Input::MouseButton button = sfmlButtonToCustomButton(event.mouseButton.button);
			if (button != Input::MouseButton::ButtonCount)
			{
				if (mouseButtonStates_[static_cast<size_t>(button)] == Input::InputState::Released)
				{
					mouseButtonStates_[static_cast<size_t>(button)] = Input::InputState::Pressed;
					buttonsJustPressed_[static_cast<size_t>(button)] = true;
					HARMONY_TRACE("Mouse button pressed: {}", static_cast<int>(button));
				}
			}
			break;
		}

		case sf::Event::MouseButtonReleased:
		{
			Input::MouseButton button = sfmlButtonToCustomButton(event.mouseButton.button);
			if (button != Input::MouseButton::ButtonCount)
			{
				mouseButtonStates_[static_cast<size_t>(button)] = Input::InputState::Released;
				buttonsJustReleased_[static_cast<size_t>(button)] = true;
				HARMONY_TRACE("Mouse button released: {}", static_cast<int>(button));
			}
			break;
		}

		case sf::Event::MouseMoved:
		{
			mouseX_ = event.mouseMove.x;
			mouseY_ = event.mouseMove.y;
			HARMONY_TRACE("Mouse moved to: ({}, {})", mouseX_, mouseY_);
			break;
		}

		case sf::Event::MouseWheelScrolled:
		{
			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
			{
				mouseWheelVerticalDelta_ += event.mouseWheelScroll.delta;
				HARMONY_TRACE("Mouse wheel vertical: {}", event.mouseWheelScroll.delta);
			}
			else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
			{
				mouseWheelHorizontalDelta_ += event.mouseWheelScroll.delta;
				HARMONY_TRACE("Mouse wheel horizontal: {}", event.mouseWheelScroll.delta);
			}
			break;
		}

		default:
			// Other event types are not input-related, ignore them
			break;
		}
	}

	void InputManager::updateStates()
	{
		// Transition pressed keys to held state
		for (size_t i = 0; i < static_cast<size_t>(Input::Key::KeyCount); ++i)
		{
			if (keyStates_[i] == Input::InputState::Pressed)
			{
				keyStates_[i] = Input::InputState::Held;
			}
			
			// Clear just-pressed and just-released flags
			keysJustPressed_[i] = false;
			keysJustReleased_[i] = false;
		}

		// Transition pressed mouse buttons to held state
		for (size_t i = 0; i < static_cast<size_t>(Input::MouseButton::ButtonCount); ++i)
		{
			if (mouseButtonStates_[i] == Input::InputState::Pressed)
			{
				mouseButtonStates_[i] = Input::InputState::Held;
			}

			// Clear just-pressed and just-released flags
			buttonsJustPressed_[i] = false;
			buttonsJustReleased_[i] = false;
		}

		// Reset mouse wheel deltas
		mouseWheelVerticalDelta_ = 0.0f;
		mouseWheelHorizontalDelta_ = 0.0f;
	}

	// ===== Keyboard Query Functions =====

	bool InputManager::isKeyPressed(Input::Key key) const
	{
		if (key == Input::Key::Unknown || key >= Input::Key::KeyCount)
			return false;

		return keysJustPressed_[static_cast<size_t>(key)];
	}

	bool InputManager::isKeyReleased(Input::Key key) const
	{
		if (key == Input::Key::Unknown || key >= Input::Key::KeyCount)
			return false;

		return keysJustReleased_[static_cast<size_t>(key)];
	}

	bool InputManager::isKeyHeld(Input::Key key) const
	{
		if (key == Input::Key::Unknown || key >= Input::Key::KeyCount)
			return false;

		Input::InputState state = keyStates_[static_cast<size_t>(key)];
		return state == Input::InputState::Pressed || state == Input::InputState::Held;
	}

	// ===== Mouse Query Functions =====

	bool InputManager::isMouseButtonPressed(Input::MouseButton button) const
	{
		if (button >= Input::MouseButton::ButtonCount)
			return false;

		return buttonsJustPressed_[static_cast<size_t>(button)];
	}

	bool InputManager::isMouseButtonReleased(Input::MouseButton button) const
	{
		if (button >= Input::MouseButton::ButtonCount)
			return false;

		return buttonsJustReleased_[static_cast<size_t>(button)];
	}

	bool InputManager::isMouseButtonHeld(Input::MouseButton button) const
	{
		if (button >= Input::MouseButton::ButtonCount)
			return false;

		Input::InputState state = mouseButtonStates_[static_cast<size_t>(button)];
		return state == Input::InputState::Pressed || state == Input::InputState::Held;
	}

	void InputManager::getMousePosition(int& x, int& y) const
	{
		x = mouseX_;
		y = mouseY_;
	}

	std::pair<int, int> InputManager::getMousePosition() const
	{
		return { mouseX_, mouseY_ };
	}

	float InputManager::getMouseWheelDelta(Input::MouseWheel wheel) const
	{
		if (wheel == Input::MouseWheel::Vertical)
			return mouseWheelVerticalDelta_;
		else
			return mouseWheelHorizontalDelta_;
	}

	// ===== Virtual Input Functions =====

	void InputManager::simulateKeyPress(Input::Key key)
	{
		if (key == Input::Key::Unknown || key >= Input::Key::KeyCount)
			return;

		// Only mark as pressed if not already held
		if (keyStates_[static_cast<size_t>(key)] == Input::InputState::Released)
		{
			keyStates_[static_cast<size_t>(key)] = Input::InputState::Pressed;
			keysJustPressed_[static_cast<size_t>(key)] = true;
			HARMONY_DEBUG("Simulated key press: {}", static_cast<int>(key));
		}
	}

	void InputManager::simulateKeyRelease(Input::Key key)
	{
		if (key == Input::Key::Unknown || key >= Input::Key::KeyCount)
			return;

		keyStates_[static_cast<size_t>(key)] = Input::InputState::Released;
		keysJustReleased_[static_cast<size_t>(key)] = true;
		HARMONY_DEBUG("Simulated key release: {}", static_cast<int>(key));
	}

	void InputManager::simulateMouseButtonPress(Input::MouseButton button)
	{
		if (button >= Input::MouseButton::ButtonCount)
			return;

		if (mouseButtonStates_[static_cast<size_t>(button)] == Input::InputState::Released)
		{
			mouseButtonStates_[static_cast<size_t>(button)] = Input::InputState::Pressed;
			buttonsJustPressed_[static_cast<size_t>(button)] = true;
			HARMONY_DEBUG("Simulated mouse button press: {}", static_cast<int>(button));
		}
	}

	void InputManager::simulateMouseButtonRelease(Input::MouseButton button)
	{
		if (button >= Input::MouseButton::ButtonCount)
			return;

		mouseButtonStates_[static_cast<size_t>(button)] = Input::InputState::Released;
		buttonsJustReleased_[static_cast<size_t>(button)] = true;
		HARMONY_DEBUG("Simulated mouse button release: {}", static_cast<int>(button));
	}

	void InputManager::simulateMouseMove(int x, int y)
	{
		mouseX_ = x;
		mouseY_ = y;
		HARMONY_DEBUG("Simulated mouse move to: ({}, {})", x, y);
	}

	void InputManager::simulateMouseWheelScroll(Input::MouseWheel wheel, float delta)
	{
		if (wheel == Input::MouseWheel::Vertical)
		{
			mouseWheelVerticalDelta_ += delta;
		}
		else
		{
			mouseWheelHorizontalDelta_ += delta;
		}
		HARMONY_DEBUG("Simulated mouse wheel scroll: {} delta {}", 
			wheel == Input::MouseWheel::Vertical ? "Vertical" : "Horizontal", delta);
	}

	void InputManager::clearAllStates()
	{
		keyStates_.fill(Input::InputState::Released);
		mouseButtonStates_.fill(Input::InputState::Released);
		keysJustPressed_.fill(false);
		keysJustReleased_.fill(false);
		buttonsJustPressed_.fill(false);
		buttonsJustReleased_.fill(false);
		mouseWheelVerticalDelta_ = 0.0f;
		mouseWheelHorizontalDelta_ = 0.0f;
		HARMONY_DEBUG("All input states cleared");
	}

	// ===== Private Helper Methods =====

	void InputManager::setKeyState(Input::Key key, Input::InputState state)
	{
		if (key != Input::Key::Unknown && key < Input::Key::KeyCount)
		{
			keyStates_[static_cast<size_t>(key)] = state;
		}
	}

	void InputManager::setMouseButtonState(Input::MouseButton button, Input::InputState state)
	{
		if (button < Input::MouseButton::ButtonCount)
		{
			mouseButtonStates_[static_cast<size_t>(button)] = state;
		}
	}

	Input::InputState InputManager::getKeyState(Input::Key key) const
	{
		if (key == Input::Key::Unknown || key >= Input::Key::KeyCount)
			return Input::InputState::Released;

		return keyStates_[static_cast<size_t>(key)];
	}

	Input::InputState InputManager::getMouseButtonState(Input::MouseButton button) const
	{
		if (button >= Input::MouseButton::ButtonCount)
			return Input::InputState::Released;

		return mouseButtonStates_[static_cast<size_t>(button)];
	}

	// Convert SFML key codes to our custom key codes
	Input::Key InputManager::sfmlKeyToCustomKey(int sfmlKey) const
	{
		// Map SFML key codes to our custom enum
		// SFML uses sf::Keyboard::Key enum values
		switch (sfmlKey)
		{
		case sf::Keyboard::A: return Input::Key::A;
		case sf::Keyboard::B: return Input::Key::B;
		case sf::Keyboard::C: return Input::Key::C;
		case sf::Keyboard::D: return Input::Key::D;
		case sf::Keyboard::E: return Input::Key::E;
		case sf::Keyboard::F: return Input::Key::F;
		case sf::Keyboard::G: return Input::Key::G;
		case sf::Keyboard::H: return Input::Key::H;
		case sf::Keyboard::I: return Input::Key::I;
		case sf::Keyboard::J: return Input::Key::J;
		case sf::Keyboard::K: return Input::Key::K;
		case sf::Keyboard::L: return Input::Key::L;
		case sf::Keyboard::M: return Input::Key::M;
		case sf::Keyboard::N: return Input::Key::N;
		case sf::Keyboard::O: return Input::Key::O;
		case sf::Keyboard::P: return Input::Key::P;
		case sf::Keyboard::Q: return Input::Key::Q;
		case sf::Keyboard::R: return Input::Key::R;
		case sf::Keyboard::S: return Input::Key::S;
		case sf::Keyboard::T: return Input::Key::T;
		case sf::Keyboard::U: return Input::Key::U;
		case sf::Keyboard::V: return Input::Key::V;
		case sf::Keyboard::W: return Input::Key::W;
		case sf::Keyboard::X: return Input::Key::X;
		case sf::Keyboard::Y: return Input::Key::Y;
		case sf::Keyboard::Z: return Input::Key::Z;

		case sf::Keyboard::Num0: return Input::Key::Num0;
		case sf::Keyboard::Num1: return Input::Key::Num1;
		case sf::Keyboard::Num2: return Input::Key::Num2;
		case sf::Keyboard::Num3: return Input::Key::Num3;
		case sf::Keyboard::Num4: return Input::Key::Num4;
		case sf::Keyboard::Num5: return Input::Key::Num5;
		case sf::Keyboard::Num6: return Input::Key::Num6;
		case sf::Keyboard::Num7: return Input::Key::Num7;
		case sf::Keyboard::Num8: return Input::Key::Num8;
		case sf::Keyboard::Num9: return Input::Key::Num9;

		case sf::Keyboard::Escape: return Input::Key::Escape;
		case sf::Keyboard::LControl: return Input::Key::LControl;
		case sf::Keyboard::LShift: return Input::Key::LShift;
		case sf::Keyboard::LAlt: return Input::Key::LAlt;
		case sf::Keyboard::LSystem: return Input::Key::LSystem;
		case sf::Keyboard::RControl: return Input::Key::RControl;
		case sf::Keyboard::RShift: return Input::Key::RShift;
		case sf::Keyboard::RAlt: return Input::Key::RAlt;
		case sf::Keyboard::RSystem: return Input::Key::RSystem;
		case sf::Keyboard::Menu: return Input::Key::Menu;

		case sf::Keyboard::LBracket: return Input::Key::LBracket;
		case sf::Keyboard::RBracket: return Input::Key::RBracket;
		case sf::Keyboard::Semicolon: return Input::Key::Semicolon;
		case sf::Keyboard::Comma: return Input::Key::Comma;
		case sf::Keyboard::Period: return Input::Key::Period;
		case sf::Keyboard::Quote: return Input::Key::Quote;
		case sf::Keyboard::Slash: return Input::Key::Slash;
		case sf::Keyboard::Backslash: return Input::Key::Backslash;
		case sf::Keyboard::Tilde: return Input::Key::Tilde;
		case sf::Keyboard::Equal: return Input::Key::Equal;
		case sf::Keyboard::Hyphen: return Input::Key::Hyphen;
		case sf::Keyboard::Space: return Input::Key::Space;
		case sf::Keyboard::Enter: return Input::Key::Enter;
		case sf::Keyboard::Backspace: return Input::Key::Backspace;
		case sf::Keyboard::Tab: return Input::Key::Tab;

		case sf::Keyboard::PageUp: return Input::Key::PageUp;
		case sf::Keyboard::PageDown: return Input::Key::PageDown;
		case sf::Keyboard::End: return Input::Key::End;
		case sf::Keyboard::Home: return Input::Key::Home;
		case sf::Keyboard::Insert: return Input::Key::Insert;
		case sf::Keyboard::Delete: return Input::Key::Delete;

		case sf::Keyboard::Add: return Input::Key::Add;
		case sf::Keyboard::Subtract: return Input::Key::Subtract;
		case sf::Keyboard::Multiply: return Input::Key::Multiply;
		case sf::Keyboard::Divide: return Input::Key::Divide;

		case sf::Keyboard::Left: return Input::Key::Left;
		case sf::Keyboard::Right: return Input::Key::Right;
		case sf::Keyboard::Up: return Input::Key::Up;
		case sf::Keyboard::Down: return Input::Key::Down;

		case sf::Keyboard::Numpad0: return Input::Key::Numpad0;
		case sf::Keyboard::Numpad1: return Input::Key::Numpad1;
		case sf::Keyboard::Numpad2: return Input::Key::Numpad2;
		case sf::Keyboard::Numpad3: return Input::Key::Numpad3;
		case sf::Keyboard::Numpad4: return Input::Key::Numpad4;
		case sf::Keyboard::Numpad5: return Input::Key::Numpad5;
		case sf::Keyboard::Numpad6: return Input::Key::Numpad6;
		case sf::Keyboard::Numpad7: return Input::Key::Numpad7;
		case sf::Keyboard::Numpad8: return Input::Key::Numpad8;
		case sf::Keyboard::Numpad9: return Input::Key::Numpad9;

		case sf::Keyboard::F1: return Input::Key::F1;
		case sf::Keyboard::F2: return Input::Key::F2;
		case sf::Keyboard::F3: return Input::Key::F3;
		case sf::Keyboard::F4: return Input::Key::F4;
		case sf::Keyboard::F5: return Input::Key::F5;
		case sf::Keyboard::F6: return Input::Key::F6;
		case sf::Keyboard::F7: return Input::Key::F7;
		case sf::Keyboard::F8: return Input::Key::F8;
		case sf::Keyboard::F9: return Input::Key::F9;
		case sf::Keyboard::F10: return Input::Key::F10;
		case sf::Keyboard::F11: return Input::Key::F11;
		case sf::Keyboard::F12: return Input::Key::F12;
		case sf::Keyboard::F13: return Input::Key::F13;
		case sf::Keyboard::F14: return Input::Key::F14;
		case sf::Keyboard::F15: return Input::Key::F15;

		case sf::Keyboard::Pause: return Input::Key::Pause;

		default:
			return Input::Key::Unknown;
		}
	}

	// Convert SFML mouse button codes to our custom button codes
	Input::MouseButton InputManager::sfmlButtonToCustomButton(int sfmlButton) const
	{
		switch (sfmlButton)
		{
		case sf::Mouse::Left: return Input::MouseButton::Left;
		case sf::Mouse::Right: return Input::MouseButton::Right;
		case sf::Mouse::Middle: return Input::MouseButton::Middle;
		case sf::Mouse::XButton1: return Input::MouseButton::XButton1;
		case sf::Mouse::XButton2: return Input::MouseButton::XButton2;
		default:
			return Input::MouseButton::ButtonCount; // Invalid button
		}
	}

} // namespace Harmony
