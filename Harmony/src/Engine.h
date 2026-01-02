#include "Harmony/Interfaces/IEngine.h"
#include "Harmony/Utilities/Clock.h"
#include "SystemsRegistry.h"

namespace Harmony 
{
    // ========================================================
    // Main Engine Class
    // ========================================================

    class Engine : public IEngine 
    {
    public:
        Engine();
        ~Engine();

		virtual void Initialize(const Properties& properties) override;

        void Run() override;
		virtual void Shutdown() override;

        State GetState() const override;
		ISystemsRegistry& GetSystemsRegistry() override;

    private:
        void HandleUpdate();
        void HandleRender();
        void HandleEvents();

    private:
		Clock m_clock;
		std::atomic<Engine::State> m_state;
    };

} // namespace Harmony