#include "Harmony/Interfaces/IEngine.h"
#include "Harmony/Utilities/Clock.h"

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

        void Run() override;

		virtual void Initialize(const Properties& properties) override;
		virtual void Shutdown() override;

        void Pause() override;
        void Resume() override;

        State GetState() const override;
        Context& GetContext() override;

    private:
        void HandleUpdate();
        void HandleRender();
        void HandleEvents();
        void WaitIfPaused();

    private:
		Clock m_clock;
		State m_state;
		Context m_context;

        mutable std::mutex m_pausingMutex;
        mutable std::shared_mutex m_stateMutex;
        std::condition_variable m_pausingCondition;
    };

} // namespace Harmony