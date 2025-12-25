#pragma once
#include "Harmony/Properties.h"
#include "Harmony/Interfaces/IPlugins.h"

#include <string>
#include <memory>
#include <functional>

namespace Harmony {
    class Event;

    class IWindow : public IPlugins {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~IWindow() = default;

        virtual void OnUpdate() = 0;
        virtual void Finalize() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
        virtual std::string GetTitle() const = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void SetFullscreen(bool enabled) = 0;
        virtual bool IsFullscreen() const = 0;

        virtual void SetResizable(bool enabled) = 0;
        virtual bool IsResizable() const = 0;

        virtual void SetTitle(const std::string& title) = 0;
        virtual void SetIcon(const std::string& path) = 0;

        virtual void SetCursorVisible(bool visible) = 0;
        virtual void SetCursorLocked(bool locked) = 0;

        // --- Native Access ---
        // CRITICAL: Plugins like ImGui or bgfx need the raw OS window handle.
        // Returns generic void* (HWND on Windows, Window* on Linux, etc.)
        virtual void* GetNativeWindow() const = 0;

        // --- Context Access ---
        // Returns the Graphics Context (OpenGL context, Vulkan instance, etc.)
        // This allows the Renderer to attach to the window.
        virtual void* GetGraphicsContext() const = 0;
    };

} // namespace Harmony