#include "pch.h"
#include "WindowHandler.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"

namespace Harmony::Internals
{
    WindowHandler::WindowHandler(const Configuration& configuration)
        : title_("Harmony Engine"), targetFPS_(60)
    {
        HARMONY_INFO("Initializing WindowHandler");

        std::optional<uint8_t> width = configuration.get<uint8_t>({ "size", "width" });
        std::optional<uint8_t> height = configuration.get<uint8_t>({ "size", "height" });
        std::optional<std::string> title = configuration.get<std::string>({ "title" });
        std::optional<bool> fullscreen = configuration.get<bool>({ "fullscreen" });
        std::optional<bool> resizable = configuration.get<bool>({ "resizable" });
        std::optional<bool> vsync = configuration.get<bool>({ "vsync" });
        std::optional<int> targetFPS = configuration.get<int>({ "targetFPS" });
        std::optional<bool> hidden = configuration.get<bool>({ "hidden" });
        std::optional<bool> borderless = configuration.get<bool>({ "borderless" });
        std::optional<bool> alwaysOnTop = configuration.get<bool>({ "alwaysOnTop" });
        std::optional<bool> transparent = configuration.get<bool>({ "transparent" });
        std::optional<bool> highdpi = configuration.get<bool>({ "highdpi" });
        std::optional<bool> msaa4x = configuration.get<bool>({ "msaa4x" });
        std::optional<bool> interlaced = configuration.get<bool>({ "interlaced" });

        int windowWidth = width.value_or(800);
        int windowHeight = height.value_or(600);

        HARMONY_ASSERT_RANGE(windowWidth, 1, 7680, "Window width out of valid range");
        HARMONY_ASSERT_RANGE(windowHeight, 1, 4320, "Window height out of valid range");

        if (windowWidth < 1 || windowWidth > 7680) {
            HARMONY_WARN("Invalid window width {}, clamping to valid range [1, 7680]", windowWidth);
            windowWidth = std::clamp(windowWidth, 1, 7680);
        }

        if (windowHeight < 1 || windowHeight > 4320) {
            HARMONY_WARN("Invalid window height {}, clamping to valid range [1, 4320]", windowHeight);
            windowHeight = std::clamp(windowHeight, 1, 4320);
        }

        if (title.has_value()) {
            std::string titleValue = title.value();
            title_ = titleValue;
        }

        if (targetFPS.has_value()) {
            int fpsValue = targetFPS.value();
            targetFPS_ = fpsValue;
            HARMONY_ASSERT_RANGE(targetFPS_, 1, 1000, "Target FPS out of valid range");
            if (targetFPS_ < 1 || targetFPS_ > 1000) {
                HARMONY_WARN("Invalid target FPS {}, clamping to valid range [1, 1000]", targetFPS_);
                targetFPS_ = std::clamp(targetFPS_, 1, 1000);
            }
        }

        HARMONY_INFO("Creating window: {}x{}, title: '{}'", windowWidth, windowHeight, title_);

        unsigned int flags = 0;
        if (resizable.value_or(false)) flags |= FLAG_WINDOW_RESIZABLE;
        if (borderless.value_or(false)) flags |= FLAG_WINDOW_UNDECORATED;
        if (hidden.value_or(false)) flags |= FLAG_WINDOW_HIDDEN;
        if (transparent.value_or(false)) flags |= FLAG_WINDOW_TRANSPARENT;
        if (highdpi.value_or(false)) flags |= FLAG_WINDOW_HIGHDPI;
        if (alwaysOnTop.value_or(false)) flags |= FLAG_WINDOW_TOPMOST;
        if (vsync.value_or(false)) flags |= FLAG_VSYNC_HINT;
        if (msaa4x.value_or(false)) flags |= FLAG_MSAA_4X_HINT;
        if (interlaced.value_or(false)) flags |= FLAG_INTERLACED_HINT;

        if (flags != 0) {
            HARMONY_DEBUG("Setting window flags: 0x{:X}", flags);
            SetConfigFlags(flags);
        }

        try {
            const char* titleCStr = title_.c_str();
            InitWindow(windowWidth, windowHeight, titleCStr);
            
            bool windowReady = IsWindowReady();
            if (!windowReady) {
                throw Exceptions::WindowInitializationException("Window failed to initialize (IsWindowReady returned false)");
            }

            HARMONY_INFO("Window initialized successfully");

            bool shouldBeFullscreen = fullscreen.value_or(false);
            if (shouldBeFullscreen) {
                HARMONY_DEBUG("Toggling fullscreen mode");
                ToggleFullscreen();
            }

            if (targetFPS_) {
                HARMONY_DEBUG("Setting target FPS to {}", targetFPS_);
                SetTargetFPS(targetFPS_);
            }
        }
        catch (const Exceptions::WindowInitializationException&) {
            throw;
        }
        catch (const std::exception& e) {
            std::string errorMessage = e.what();
            HARMONY_CRITICAL("Window initialization failed with exception: {}", errorMessage);
            throw Exceptions::WindowInitializationException(errorMessage);
        }
    }

    WindowHandler::~WindowHandler() {
        HARMONY_INFO("Destroying WindowHandler");
        bool shouldClose = WindowShouldClose();
        bool isReady = IsWindowReady();
        if (!shouldClose && isReady) {
            CloseWindow();
            HARMONY_DEBUG("Window closed");
        }
    }

    void WindowHandler::close() {
        HARMONY_INFO("Closing window");
        CloseWindow();
    }

    bool WindowHandler::shouldClose() const {
        return WindowShouldClose();
    }

    void WindowHandler::setSize(Vector2i size) {
        HARMONY_ASSERT_RANGE(size.x, 1, 7680, "Window width out of valid range");
        HARMONY_ASSERT_RANGE(size.y, 1, 4320, "Window height out of valid range");
        HARMONY_DEBUG("Setting window size to {}x{}", size.x, size.y);
        SetWindowSize(size.x, size.y);
    }

    Vector2i WindowHandler::getSize() const {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        return { screenWidth, screenHeight };
    }

    void WindowHandler::setMinSize(Vector2i minSize) {
        HARMONY_ASSERT_RANGE(minSize.x, 1, 7680, "Minimum window width out of valid range");
        HARMONY_ASSERT_RANGE(minSize.y, 1, 4320, "Minimum window height out of valid range");
        HARMONY_DEBUG("Setting minimum window size to {}x{}", minSize.x, minSize.y);
        SetWindowMinSize(minSize.x, minSize.y);
    }

    void WindowHandler::setMaxSize(Vector2i maxSize) {
        HARMONY_ASSERT_RANGE(maxSize.x, 1, 7680, "Maximum window width out of valid range");
        HARMONY_ASSERT_RANGE(maxSize.y, 1, 4320, "Maximum window height out of valid range");
        HARMONY_DEBUG("Setting maximum window size to {}x{}", maxSize.x, maxSize.y);
        SetWindowMaxSize(maxSize.x, maxSize.y);
    }

    void WindowHandler::setPosition(Vector2i position) {
        HARMONY_DEBUG("Setting window position to ({}, {})", position.x, position.y);
        SetWindowPosition(position.x, position.y);
    }

    Vector2f WindowHandler::getPosition() const {
        Vector2 windowPosition = GetWindowPosition();
        return { windowPosition.x, windowPosition.y };
    }

    void WindowHandler::setTitle(const std::string& title) {
        bool titleEmpty = title.empty();
        HARMONY_ASSERT(!titleEmpty, "Window title cannot be empty");
        HARMONY_DEBUG("Setting window title to '{}'", title);
        title_ = title;
        const char* titleCStr = title.c_str();
        SetWindowTitle(titleCStr);
    }

    std::string WindowHandler::getTitle() const {
        return title_;
    }

    bool WindowHandler::isReady() const {
        return IsWindowReady();
    }

    bool WindowHandler::isFocused() const {
        return IsWindowFocused();
    }

    bool WindowHandler::isFullscreen() const {
        return IsWindowFullscreen();
    }

    bool WindowHandler::isHidden() const {
        return IsWindowHidden();
    }

    bool WindowHandler::isMinimized() const {
        return IsWindowMinimized();
    }

    bool WindowHandler::isMaximized() const {
        return IsWindowMaximized();
    }

    bool WindowHandler::isResizable() const {
        unsigned int resizableFlag = FLAG_WINDOW_RESIZABLE;
        return IsWindowState(resizableFlag);
    }

    bool WindowHandler::isVSyncEnabled() const {
        unsigned int vsyncFlag = FLAG_VSYNC_HINT;
        return IsWindowState(vsyncFlag);
    }

    void WindowHandler::toggleFullscreen() {
        HARMONY_DEBUG("Toggling fullscreen mode");
        ToggleFullscreen();
    }

    void WindowHandler::maximize() {
        HARMONY_DEBUG("Maximizing window");
        MaximizeWindow();
    }

    void WindowHandler::minimize() {
        HARMONY_DEBUG("Minimizing window");
        MinimizeWindow();
    }

    void WindowHandler::restore() {
        HARMONY_DEBUG("Restoring window");
        RestoreWindow();
    }

    void WindowHandler::setResizable(bool resizable) {
        HARMONY_DEBUG("Setting window resizable: {}", resizable);
        unsigned int resizableFlag = FLAG_WINDOW_RESIZABLE;
        if (resizable) {
            SetWindowState(resizableFlag);
        } else {
            ClearWindowState(resizableFlag);
        }
    }

    void WindowHandler::setVSync(bool vsync) {
        HARMONY_DEBUG("Setting VSync: {}", vsync);
        unsigned int vsyncFlag = FLAG_VSYNC_HINT;
        if (vsync) {
            SetWindowState(vsyncFlag);
        } else {
            ClearWindowState(vsyncFlag);
        }
    }

    void WindowHandler::setBorderless(bool borderless) {
        HARMONY_DEBUG("Setting borderless: {}", borderless);
        unsigned int borderlessFlag = FLAG_WINDOW_UNDECORATED;
        if (borderless) {
            SetWindowState(borderlessFlag);
        } else {
            ClearWindowState(borderlessFlag);
        }
    }

    void WindowHandler::setAlwaysOnTop(bool alwaysOnTop) {
        HARMONY_DEBUG("Setting always on top: {}", alwaysOnTop);
        unsigned int topMostFlag = FLAG_WINDOW_TOPMOST;
        if (alwaysOnTop) {
            SetWindowState(topMostFlag);
        } else {
            ClearWindowState(topMostFlag);
        }
    }

    void WindowHandler::setTransparent(bool transparent) {
        HARMONY_DEBUG("Setting transparent: {}", transparent);
        unsigned int transparentFlag = FLAG_WINDOW_TRANSPARENT;
        if (transparent) {
            SetWindowState(transparentFlag);
        } else {
            ClearWindowState(transparentFlag);
        }
    }

    void WindowHandler::showCursor() {
        HARMONY_TRACE("Showing cursor");
        ShowCursor();
    }

    void WindowHandler::hideCursor() {
        HARMONY_TRACE("Hiding cursor");
        HideCursor();
    }

    bool WindowHandler::isCursorHidden() const {
        return IsCursorHidden();
    }

    bool WindowHandler::isCursorOnScreen() const {
        return IsCursorOnScreen();
    }

    void WindowHandler::setTargetFPS(int fps) {
        HARMONY_ASSERT_RANGE(fps, 0, 1000, "Target FPS out of valid range");
        if (fps < 0 || fps > 1000) {
            HARMONY_WARN("Invalid target FPS {}, clamping to valid range [0, 1000]", fps);
            fps = std::clamp(fps, 0, 1000);
        }
        HARMONY_DEBUG("Setting target FPS to {}", fps);
        targetFPS_ = fps;
        SetTargetFPS(fps);
    }

    int WindowHandler::getTargetFPS() const {
        return targetFPS_;
    }

    float WindowHandler::getFrameTime() const {
        return GetFrameTime();
    }

    double WindowHandler::getTime() const {
        return GetTime();
    }

    int WindowHandler::getCurrentFPS() const {
        return GetFPS();
    }

    int WindowHandler::getMonitorCount() const {
        return GetMonitorCount();
    }

    int WindowHandler::getCurrentMonitor() const {
        return GetCurrentMonitor();
    }

    Vector2i WindowHandler::getMonitorSize(int monitor) const {
        int monitorCount = GetMonitorCount();
        int maxMonitorIndex = monitorCount - 1;
        HARMONY_ASSERT_RANGE(monitor, 0, maxMonitorIndex, "Monitor index out of range");
        int monitorWidth = GetMonitorWidth(monitor);
        int monitorHeight = GetMonitorHeight(monitor);
        return { monitorWidth, monitorHeight };
    }
}