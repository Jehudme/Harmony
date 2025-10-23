# Input System Quick Reference

## Basic Usage

### Accessing the InputManager

```cpp
// In a script
auto& input = getScene().engine.inputManager;

// In engine code
auto& input = engine.inputManager;
```

## Keyboard Input

### Query Functions

```cpp
// Just pressed this frame (one-time action)
if (input->isKeyPressed(Input::Key::Space)) {
    jump();
}

// Held down (continuous action)
if (input->isKeyHeld(Input::Key::W)) {
    moveForward();
}

// Just released this frame
if (input->isKeyReleased(Input::Key::W)) {
    stopMoving();
}
```

### Common Keys

```cpp
// Letters: Input::Key::A through Input::Key::Z
// Numbers: Input::Key::Num0 through Input::Key::Num9
// Arrows: Input::Key::Left, Right, Up, Down
// Modifiers: Input::Key::LShift, RShift, LControl, RControl, LAlt, RAlt
// Function: Input::Key::F1 through Input::Key::F15
// Special: Input::Key::Escape, Space, Enter, Tab, Backspace
```

## Mouse Input

### Mouse Buttons

```cpp
// Left click
if (input->isMouseButtonPressed(Input::MouseButton::Left)) {
    handleClick();
}

// Right click held
if (input->isMouseButtonHeld(Input::MouseButton::Right)) {
    aim();
}

// Available buttons: Left, Right, Middle, XButton1, XButton2
```

### Mouse Position

```cpp
// Method 1: Pair
auto [x, y] = input->getMousePosition();

// Method 2: References
int x, y;
input->getMousePosition(x, y);
```

### Mouse Wheel

```cpp
// Vertical scroll
float delta = input->getMouseWheelDelta(Input::MouseWheel::Vertical);
if (delta > 0) {
    zoomIn();
} else if (delta < 0) {
    zoomOut();
}

// Horizontal scroll
float hDelta = input->getMouseWheelDelta(Input::MouseWheel::Horizontal);
```

## Virtual Input (Testing/Automation)

### Keyboard Simulation

```cpp
// Press a key
input->simulateKeyPress(Input::Key::W);

// Release a key
input->simulateKeyRelease(Input::Key::W);
```

### Mouse Simulation

```cpp
// Move mouse
input->simulateMouseMove(100, 200);

// Click
input->simulateMouseButtonPress(Input::MouseButton::Left);
input->simulateMouseButtonRelease(Input::MouseButton::Left);

// Scroll
input->simulateMouseWheelScroll(Input::MouseWheel::Vertical, 1.0f);
```

### Clear All Input

```cpp
// Clear all input states (useful for state transitions)
input->clearAllStates();
```

## Common Patterns

### Player Movement (WASD)

```cpp
void onPreUpdate() override {
    auto& input = getScene().engine.inputManager;
    auto& transform = getScene().componentReference<Transform>(entityId);
    
    float speed = 200.0f * getScene().engine.getDeltaTime();
    float x, y;
    transform.getPosition(x, y);
    
    if (input->isKeyHeld(Input::Key::W)) y -= speed;
    if (input->isKeyHeld(Input::Key::S)) y += speed;
    if (input->isKeyHeld(Input::Key::A)) x -= speed;
    if (input->isKeyHeld(Input::Key::D)) x += speed;
    
    transform.setPosition(x, y);
}
```

### Jump Action

```cpp
void onPreUpdate() override {
    auto& input = getScene().engine.inputManager;
    
    // Use isKeyPressed for one-time action
    if (input->isKeyPressed(Input::Key::Space)) {
        performJump();
    }
}
```

### Mouse Click Detection

```cpp
void onPreUpdate() override {
    auto& input = getScene().engine.inputManager;
    
    if (input->isMouseButtonPressed(Input::MouseButton::Left)) {
        auto [x, y] = input->getMousePosition();
        handleClickAt(x, y);
    }
}
```

### Combo Input (Ctrl+S)

```cpp
void onPreUpdate() override {
    auto& input = getScene().engine.inputManager;
    
    if (input->isKeyHeld(Input::Key::LControl) && 
        input->isKeyPressed(Input::Key::S)) {
        saveGame();
    }
}
```

### Follow Mouse Cursor

```cpp
void onPreUpdate() override {
    auto& input = getScene().engine.inputManager;
    auto& transform = getScene().componentReference<Transform>(entityId);
    
    auto [x, y] = input->getMousePosition();
    transform.setPosition(static_cast<float>(x), static_cast<float>(y));
}
```

## Input State Lifecycle

```
Frame 1: Key Down Event
  - isKeyPressed() = true
  - isKeyHeld() = true
  - isKeyReleased() = false

Frame 2-N: Key Still Down (no event)
  - isKeyPressed() = false
  - isKeyHeld() = true
  - isKeyReleased() = false

Frame N+1: Key Up Event
  - isKeyPressed() = false
  - isKeyHeld() = false
  - isKeyReleased() = true

Frame N+2: No Input
  - isKeyPressed() = false
  - isKeyHeld() = false
  - isKeyReleased() = false
```

## Best Practices

1. **Use `isKeyPressed()` for one-time actions**: jump, shoot, menu selection
2. **Use `isKeyHeld()` for continuous actions**: movement, aiming
3. **Use `isKeyReleased()` for release-triggered actions**: charge attacks
4. **Clear states on state transitions**: prevent input carryover between menus/gameplay
5. **Combine keys for shortcuts**: Ctrl+S, Alt+F4, Shift+Click
6. **Use virtual input for testing**: automate gameplay testing

## Complete Key Enum Reference

### Letters (A-Z)
`Input::Key::A` through `Input::Key::Z`

### Numbers (0-9)
`Input::Key::Num0` through `Input::Key::Num9`

### Function Keys (F1-F15)
`Input::Key::F1` through `Input::Key::F15`

### Arrow Keys
`Input::Key::Left`, `Input::Key::Right`, `Input::Key::Up`, `Input::Key::Down`

### Numpad
`Input::Key::Numpad0` through `Input::Key::Numpad9`
`Input::Key::Add`, `Input::Key::Subtract`, `Input::Key::Multiply`, `Input::Key::Divide`

### Modifiers
`Input::Key::LShift`, `Input::Key::RShift`
`Input::Key::LControl`, `Input::Key::RControl`
`Input::Key::LAlt`, `Input::Key::RAlt`
`Input::Key::LSystem`, `Input::Key::RSystem`

### Special Keys
`Input::Key::Escape`, `Input::Key::Space`, `Input::Key::Enter`
`Input::Key::Backspace`, `Input::Key::Tab`
`Input::Key::PageUp`, `Input::Key::PageDown`
`Input::Key::Home`, `Input::Key::End`
`Input::Key::Insert`, `Input::Key::Delete`
`Input::Key::Pause`

### Punctuation
`Input::Key::LBracket`, `Input::Key::RBracket`
`Input::Key::Semicolon`, `Input::Key::Comma`, `Input::Key::Period`
`Input::Key::Quote`, `Input::Key::Slash`, `Input::Key::Backslash`
`Input::Key::Tilde`, `Input::Key::Equal`, `Input::Key::Hyphen`

## Mouse Button Enum Reference

```cpp
Input::MouseButton::Left      // Left mouse button
Input::MouseButton::Right     // Right mouse button  
Input::MouseButton::Middle    // Middle mouse button (wheel click)
Input::MouseButton::XButton1  // Extra button 1
Input::MouseButton::XButton2  // Extra button 2
```

## Notes

- Input states are automatically updated at the end of each frame
- All query functions are const and thread-safe for reading
- Virtual input follows the same state machine as real input
- Mouse position is in window coordinates (0,0 = top-left)
- Mouse wheel delta is reset to 0 after each frame
