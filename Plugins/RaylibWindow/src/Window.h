#pragma once

#include <Harmony/IWindow.h>
#include <string>

namespace Harmony
{

class RaylibWindow : public IWindow
{
public:
    RaylibWindow();
    ~RaylibWindow() override;

    // --- ISystem Interface ---
    State GetState() const override;
    std::string GetName() const override;
    std::string GetRole() const override;
    std::string GetVersion() const override;
    void Initialize(const Properties& properties) override;
    void Finalize() override;

    // --- IWindow Interface ---
    void OnUpdate() override;

    uint32_t GetWidth() const override;
    uint32_t GetHeight() const override;
    std::pair<uint32_t, uint32_t> GetSize() const override;
    std::string GetTitle() const override;

    void SetEventCallback(const EventCallbackFn& callback) override;
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    void SetFullscreen(bool enabled) override;
    bool IsFullscreen() const override;

    void SetResizable(bool enabled) override;
    bool IsResizable() const override;

    void SetTitle(const std::string& title) override;
    void SetIcon(const std::string& path) override;

    void SetCursorVisible(bool visible) override;
    void SetCursorLocked(bool locked) override;

    void* GetNativeWindow() const override;
    void* GetGraphicsContext() const override;

private:
    State m_state = State::Uninitialized;
    
    uint32_t m_width = 1280;
    uint32_t m_height = 720;
    std::string m_title = "Harmony Engine";
    bool m_vsync = true;
    bool m_fullscreen = false;
    bool m_resizable = true;
    
    EventCallbackFn m_eventCallback;
};

} // namespace Harmony
