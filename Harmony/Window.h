#pragma once
#include "Configuration.h"

#include <string>
#include "Vector.h"

namespace Harmony::Internals {

    class Window
    {
    public:
        explicit Window(const Configuration& configuration);
        ~Window();

        void close();
        bool shouldClose() const;

        // Size and position
        void setSize(Vector2i size);
        Vector2i getSize() const;

        void setPosition(Vector2i position);
        Vector2f getPosition() const;

        // Title
        void setTitle(const std::string& title);

        // State
        bool isReady() const;
        bool isFocused() const;
        bool isFullscreen() const;
        bool isHidden() const;
        bool isMinimized() const;
        bool isMaximized() const;

        // Control
        void toggleFullscreen();
        void maximize();
        void minimize();
        void restore();

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
    };

} // namespace Harmony::Internals

