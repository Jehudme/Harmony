#pragma once
#include "Configuration.h"

#include <string>
#include "Vector.h"

namespace Harmony::Internals {

    class WindowHandler
    {
    public:
        explicit WindowHandler(const Configuration& configuration);
        ~WindowHandler();

        void close();
        bool shouldClose() const;

        // Size and position
        void setSize(Vector2i size);
        Vector2i getSize() const;
        void setMinSize(Vector2i minSize);
        void setMaxSize(Vector2i maxSize);

        void setPosition(Vector2i position);
        Vector2f getPosition() const;

        // Title and icon
        void setTitle(const std::string& title);
        std::string getTitle() const;

        // State
        bool isReady() const;
        bool isFocused() const;
        bool isFullscreen() const;
        bool isHidden() const;
        bool isMinimized() const;
        bool isMaximized() const;
        bool isResizable() const;
        bool isVSyncEnabled() const;

        // Control
        void toggleFullscreen();
        void maximize();
        void minimize();
        void restore();
        void setResizable(bool resizable);
        void setVSync(bool vsync);
        void setBorderless(bool borderless);
        void setAlwaysOnTop(bool alwaysOnTop);
        void setTransparent(bool transparent);

        // Cursor
        void showCursor();
        void hideCursor();
        bool isCursorHidden() const;
        bool isCursorOnScreen() const;

        // Frame timing
        void setTargetFPS(int fps);
        int getTargetFPS() const;
        float getFrameTime() const;
        double getTime() const;
        int getCurrentFPS() const;

        // Monitor information
        int getMonitorCount() const;
        int getCurrentMonitor() const;
        Vector2i getMonitorSize(int monitor = 0) const;

    private:
        std::string title_;
        int targetFPS_;
    };

} // namespace Harmony::Internals

