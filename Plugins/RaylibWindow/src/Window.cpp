#include "Window.h"
#include <Harmony/Logger.h>
#include <Harmony/Assert.h>
#include <Harmony/Properties.h>
#include <Harmony/PluginsFactoriesRegistry.h>
#include <raylib.h>

namespace Harmony
{

RaylibWindow::RaylibWindow()
{
    HARMONY_INFO("RaylibWindow: Constructor called");
}

RaylibWindow::~RaylibWindow()
{
    HARMONY_INFO("RaylibWindow: Destructor called");
}

// --- ISystem Interface ---

ISystem::State RaylibWindow::GetState() const
{
    return m_state;
}

std::string RaylibWindow::GetName() const
{
    return "RaylibWindow";
}

std::string RaylibWindow::GetRole() const
{
    return "Window";
}

std::string RaylibWindow::GetVersion() const
{
    return "1.0.0";
}

void RaylibWindow::Initialize(const Properties& properties)
{
    HARMONY_INFO("RaylibWindow: Initializing...");

    // Extract properties
    auto width = properties.Get<int>({"width"});
    auto height = properties.Get<int>({"height"});
    auto title = properties.Get<std::string>({"title"});
    auto vsync = properties.Get<bool>({"vsync"});
    auto fullscreen = properties.Get<bool>({"fullscreen"});
    auto resizable = properties.Get<bool>({"resizable"});

    // Apply properties with defaults
    if (width.has_value())
    {
        m_width = static_cast<uint32_t>(width.value());
        HARMONY_INFO("RaylibWindow: Width set to {}", m_width);
    }

    if (height.has_value())
    {
        m_height = static_cast<uint32_t>(height.value());
        HARMONY_INFO("RaylibWindow: Height set to {}", m_height);
    }

    if (title.has_value())
    {
        m_title = title.value();
        HARMONY_INFO("RaylibWindow: Title set to '{}'", m_title);
    }

    if (vsync.has_value())
    {
        m_vsync = vsync.value();
        HARMONY_INFO("RaylibWindow: VSync set to {}", m_vsync);
    }

    if (fullscreen.has_value())
    {
        m_fullscreen = fullscreen.value();
        HARMONY_INFO("RaylibWindow: Fullscreen set to {}", m_fullscreen);
    }

    if (resizable.has_value())
    {
        m_resizable = resizable.value();
        HARMONY_INFO("RaylibWindow: Resizable set to {}", m_resizable);
    }

    // Set window configuration flags before InitWindow
    unsigned int flags = 0;
    
    if (m_vsync)
    {
        flags |= FLAG_VSYNC_HINT;
    }
    
    if (m_fullscreen)
    {
        flags |= FLAG_FULLSCREEN_MODE;
    }
    
    if (m_resizable)
    {
        flags |= FLAG_WINDOW_RESIZABLE;
    }

    if (flags != 0)
    {
        SetConfigFlags(flags);
        HARMONY_INFO("RaylibWindow: Window flags configured");
    }

    // Initialize the window
    InitWindow(m_width, m_height, m_title.c_str());
    
    if (!IsWindowReady())
    {
        HARMONY_ERROR("RaylibWindow: Failed to create window");
        m_state = State::Uninitialized;
        return;
    }

    HARMONY_INFO("RaylibWindow: Window created successfully ({}x{}, '{}')", m_width, m_height, m_title);

    // Apply VSync after window creation
    if (m_vsync)
    {
        SetTargetFPS(60);
        HARMONY_INFO("RaylibWindow: Target FPS set to 60 (VSync)");
    }

    m_state = State::Initialized;
    HARMONY_INFO("RaylibWindow: Initialization complete");
}

void RaylibWindow::Finalize()
{
    HARMONY_INFO("RaylibWindow: Finalizing...");
    
    if (IsWindowReady())
    {
        CloseWindow();
        HARMONY_INFO("RaylibWindow: Window closed");
    }
    
    m_state = State::Uninitialized;
    HARMONY_INFO("RaylibWindow: Finalization complete");
}

// --- IWindow Interface ---

void RaylibWindow::OnUpdate()
{
    // Poll events and swap buffers
    // Raylib handles this internally
}

uint32_t RaylibWindow::GetWidth() const
{
    if (IsWindowReady())
    {
        return static_cast<uint32_t>(GetScreenWidth());
    }
    return m_width;
}

uint32_t RaylibWindow::GetHeight() const
{
    if (IsWindowReady())
    {
        return static_cast<uint32_t>(GetScreenHeight());
    }
    return m_height;
}

std::pair<uint32_t, uint32_t> RaylibWindow::GetSize() const
{
    return {GetWidth(), GetHeight()};
}

std::string RaylibWindow::GetTitle() const
{
    return m_title;
}

void RaylibWindow::SetEventCallback(const EventCallbackFn& callback)
{
    m_eventCallback = callback;
    HARMONY_INFO("RaylibWindow: Event callback set");
}

void RaylibWindow::SetVSync(bool enabled)
{
    m_vsync = enabled;
    
    if (IsWindowReady())
    {
        if (enabled)
        {
            SetTargetFPS(60);
            HARMONY_INFO("RaylibWindow: VSync enabled (60 FPS)");
        }
        else
        {
            SetTargetFPS(0);
            HARMONY_INFO("RaylibWindow: VSync disabled (unlimited FPS)");
        }
    }
}

bool RaylibWindow::IsVSync() const
{
    return m_vsync;
}

void RaylibWindow::SetFullscreen(bool enabled)
{
    if (IsWindowReady())
    {
        if (enabled != m_fullscreen)
        {
            ToggleFullscreen();
            m_fullscreen = enabled;
            HARMONY_INFO("RaylibWindow: Fullscreen toggled to {}", enabled);
        }
    }
    else
    {
        m_fullscreen = enabled;
    }
}

bool RaylibWindow::IsFullscreen() const
{
    if (IsWindowReady())
    {
        return IsWindowFullscreen();
    }
    return m_fullscreen;
}

void RaylibWindow::SetResizable(bool enabled)
{
    m_resizable = enabled;
    HARMONY_WARN("RaylibWindow: SetResizable not supported at runtime, must be set before initialization");
}

bool RaylibWindow::IsResizable() const
{
    return m_resizable;
}

void RaylibWindow::SetTitle(const std::string& title)
{
    m_title = title;
    
    if (IsWindowReady())
    {
        SetWindowTitle(title.c_str());
        HARMONY_INFO("RaylibWindow: Title changed to '{}'", title);
    }
}

void RaylibWindow::SetIcon(const std::string& path)
{
    if (IsWindowReady())
    {
        Image icon = LoadImage(path.c_str());
        if (icon.data != nullptr)
        {
            SetWindowIcon(icon);
            UnloadImage(icon);
            HARMONY_INFO("RaylibWindow: Icon set from '{}'", path);
        }
        else
        {
            HARMONY_WARN("RaylibWindow: Failed to load icon from '{}'", path);
        }
    }
}

void RaylibWindow::SetCursorVisible(bool visible)
{
    if (IsWindowReady())
    {
        if (visible)
        {
            ShowCursor();
            HARMONY_INFO("RaylibWindow: Cursor shown");
        }
        else
        {
            HideCursor();
            HARMONY_INFO("RaylibWindow: Cursor hidden");
        }
    }
}

void RaylibWindow::SetCursorLocked(bool locked)
{
    if (IsWindowReady())
    {
        if (locked)
        {
            DisableCursor();
            HARMONY_INFO("RaylibWindow: Cursor locked");
        }
        else
        {
            EnableCursor();
            HARMONY_INFO("RaylibWindow: Cursor unlocked");
        }
    }
}

void* RaylibWindow::GetNativeWindow() const
{
    // Raylib doesn't expose the native window handle directly in a cross-platform way
    // This would need platform-specific code
    HARMONY_WARN("RaylibWindow: GetNativeWindow not fully implemented");
    return nullptr;
}

void* RaylibWindow::GetGraphicsContext() const
{
    // Raylib doesn't expose the graphics context directly
    // This would need platform-specific code
    HARMONY_WARN("RaylibWindow: GetGraphicsContext not fully implemented");
    return nullptr;
}

} // namespace Harmony

// Register the plugin
HARMONY_REGISTER_SYSTEM(Harmony::RaylibWindow, "RaylibWindow");
