#include "Harmony/Interfaces/IEngine.h"
#include "Harmony/Utilities/Clock.h"

#include "PluginsRegistry.h"

namespace Harmony {
	class Engine : public IEngine {
	public:
		Engine(const Properties& properties);
		~Engine();

		void Run() override;

		void Pause() override;
		void Resume() override;

		State GetState() const override;
		Context& GetContext() override;

	private:
		void inline handleUpdate();
		void inline handleRender();
		void inline handleEvents();

		void inline waitIfPaused();

		struct Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace Harmony