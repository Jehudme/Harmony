#pragma once
#include "Properties.h"

#include <string>
#include <memory>
#include <functional>

namespace Harmony {
    class Event;

    // --- 2. The Interface ---
    class IWindow {
    public:
        // Define a standard callback function type for events
        // The Engine will bind a function here to receive input/window events.
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~IWindow() = default;

        // --- Lifecycle ---
        virtual void OnUpdate() = 0; // Poll events, Swap buffers
        virtual void Shutdown() = 0;

        // --- Attributes (Getters) ---
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
        virtual std::string GetTitle() const = 0;

        // --- Attributes (Setters) ---
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void SetFullscreen(bool enabled) = 0;
        virtual bool IsFullscreen() const = 0;

        virtual void SetResizable(bool enabled) = 0;
        virtual bool IsResizable() const = 0;

        virtual void SetTitle(const std::string& title) = 0;

        // Sets the window icon (path or raw data, usually path for simplicity first)
        virtual void SetIcon(const std::string& path) = 0;

        // --- Cursor Management ---
        // Vital for FPS games (Hide cursor) vs RTS games (Show cursor)
        virtual void SetCursorVisible(bool visible) = 0;
        virtual void SetCursorLocked(bool locked) = 0; // Locks cursor to center

        // --- Native Access ---
        // CRITICAL: Plugins like ImGui or bgfx need the raw OS window handle.
        // Returns generic void* (HWND on Windows, Window* on Linux, etc.)
        virtual void* GetNativeWindow() const = 0;

        // --- Context Access ---
        // Returns the Graphics Context (OpenGL context, Vulkan instance, etc.)
        // This allows the Renderer to attach to the window.
        virtual void* GetGraphicsContext() const = 0;

        // --- Static Factory ---
        // Implemented in .cpp based on compile definitions (RAYLIB, GLFW, SDL)
        static std::unique_ptr<IWindow> Create(const Properties& properties);
    };

} // namespace Harmony