#include "pch.h"
#include "WindowHandler.h"

namespace Harmony::Internals
{
    WindowHandler::WindowHandler(const Configuration& configuration)
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

    WindowHandler::~WindowHandler()                        { if (!WindowShouldClose()) CloseWindow(); }

    void WindowHandler::close()                            { CloseWindow(); }
    bool WindowHandler::shouldClose() const                { return WindowShouldClose(); }

    void WindowHandler::setSize(Vector2i size)             { SetWindowSize(size.x, size.y); }
    Vector2i WindowHandler::getSize() const                { return { GetScreenWidth(), GetScreenHeight() }; }
    void WindowHandler::setPosition(Vector2i position)     { SetWindowPosition(position.x, position.y); }
    Vector2f WindowHandler::getPosition() const            { return { GetWindowPosition().x, GetWindowPosition().y }; }
    void WindowHandler::setTitle(const std::string& title) { SetWindowTitle(title.c_str()); }

    bool WindowHandler::isReady() const                    { return IsWindowReady(); }
    bool WindowHandler::isFocused() const                  { return IsWindowFocused(); }
    bool WindowHandler::isFullscreen() const               { return IsWindowFullscreen(); }
    bool WindowHandler::isHidden() const                   { return IsWindowHidden(); }
    bool WindowHandler::isMinimized() const                { return IsWindowMinimized(); }
    bool WindowHandler::isMaximized() const                { return IsWindowMaximized(); }

    void WindowHandler::toggleFullscreen()                 { ToggleFullscreen(); }
    void WindowHandler::maximize()                         { MaximizeWindow(); }
    void WindowHandler::minimize()                         { MinimizeWindow(); }
    void WindowHandler::restore()                          { RestoreWindow(); }

    void WindowHandler::showCursor()                       { ShowCursor(); }
    void WindowHandler::hideCursor()                       { HideCursor(); }
    bool WindowHandler::isCursorHidden() const             { return IsCursorHidden(); }
    bool WindowHandler::isCursorOnScreen() const           { return IsCursorOnScreen(); }

    void WindowHandler::setTargetFPS(int fps)              { SetTargetFPS(fps); }
    int WindowHandler::getTargetFPS() const                { return GetFPS(); }
    float WindowHandler::getFrameTime() const              { return GetFrameTime(); }
    double WindowHandler::getTime() const                  { return GetTime(); }
}