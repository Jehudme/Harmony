#include "pch.h"
#include "Window.h"

namespace Harmony::Internals
{
    Window::Window(const Configuration& configuration)
    {
        // Read configuration values
        std::optional<int> width                    = configuration.get<uint8_t>({ "size", "width" });
        std::optional<int> height                   = configuration.get<uint8_t>({ "size", "height" });
        std::optional<std::string> title            = configuration.get<std::string>({ "title" });
        std::optional<bool> fullscreen              = configuration.get<bool>({ "fullscreen" });
        std::optional<bool> resizable               = configuration.get<bool>({ "resizable" });
        std::optional<bool> vsync                   = configuration.get<bool>({ "vsync" });
        std::optional<int> targetFPS                = configuration.get<int>({ "targetFPS" });
        std::optional<bool> hidden                  = configuration.get<bool>({ "hidden" });

        // Initialize window
        InitWindow(width.value_or(800), height.value_or(600), title.value_or("Harmony Engine").c_str());

        // Apply flags
        if (fullscreen.value_or(false))             ToggleFullscreen();
        if (resizable.value_or(false))              SetWindowState(FLAG_WINDOW_RESIZABLE);
        if (hidden.value_or(false))                 SetWindowState(FLAG_WINDOW_HIDDEN);
        if (targetFPS.has_value())                  SetTargetFPS(targetFPS.value());
        if (vsync.value_or(false))                  SetWindowState(FLAG_VSYNC_HINT);
        else                                        ClearWindowState(FLAG_VSYNC_HINT);
    }

    Window::~Window()                               { if (!WindowShouldClose()) CloseWindow(); }

    void Window::close()                            { CloseWindow(); }
    bool Window::shouldClose() const                { return WindowShouldClose(); }

    void Window::setSize(Vector2i size)             { SetWindowSize(size.x, size.y); }
    Vector2i Window::getSize() const                { return { GetScreenWidth(), GetScreenHeight() }; }
    void Window::setPosition(Vector2i position)     { SetWindowPosition(position.x, position.y); }
    Vector2f Window::getPosition() const            { return { GetWindowPosition().x, GetWindowPosition().y }; }
    void Window::setTitle(const std::string& title) { SetWindowTitle(title.c_str()); }

    bool Window::isReady() const                    { return IsWindowReady(); }
    bool Window::isFocused() const                  { return IsWindowFocused(); }
    bool Window::isFullscreen() const               { return IsWindowFullscreen(); }
    bool Window::isHidden() const                   { return IsWindowHidden(); }
    bool Window::isMinimized() const                { return IsWindowMinimized(); }
    bool Window::isMaximized() const                { return IsWindowMaximized(); }

    void Window::toggleFullscreen()                 { ToggleFullscreen(); }
    void Window::maximize()                         { MaximizeWindow(); }
    void Window::minimize()                         { MinimizeWindow(); }
    void Window::restore()                          { RestoreWindow(); }

    void Window::showCursor()                       { ShowCursor(); }
    void Window::hideCursor()                       { HideCursor(); }
    bool Window::isCursorHidden() const             { return IsCursorHidden(); }
    bool Window::isCursorOnScreen() const           { return IsCursorOnScreen(); }

    void Window::setTargetFPS(int fps)              { SetTargetFPS(fps); }
    int Window::getTargetFPS() const                { return GetFPS(); }
    float Window::getFrameTime() const              { return GetFrameTime(); }
    double Window::getTime() const                  { return GetTime(); }
}