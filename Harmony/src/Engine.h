#include "Harmony/Interfaces/IEngine.h"
#include "Harmony/Utilities/Clock.h"

#include "PluginsRegistry.h"

namespace Harmony 
{
    // ========================================================
    // Main Engine Class
    // ========================================================

    class Engine : public IEngine 
    {
    public:
        Engine(const Properties& properties);
        ~Engine();

        void Run() override;

        void Pause() override;
        void Resume() override;

        State GetState() const override;
        Context& GetContext() override;

    private:
        void HandleUpdate();
        void HandleRender();
        void HandleEvents();
        void WaitIfPaused();

        struct Internal;
        std::unique_ptr<Internal> m_internal;
    };

} // namespace Harmony