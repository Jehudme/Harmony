
#include <Harmony/Engine.h>
#include <Harmony/Logger.h>
#include <Harmony/ScriptComponent.h>
#include <Harmony/Scene.h>
#include <raylib.h>
#include <raymath.h>
#include <Harmony/View3DComponent.h>

using namespace Harmony;
using namespace Harmony::Internals;

int main()
{
	Harmony::Logger::initialize();
	Configuration config;
	config.load("properties.json");
	config.debugPrint();

	Engine(config).start();
}

namespace Sandbox {
	class ScriptComponent : public Harmony::Components::ScriptComponent {
	public:
		ScriptComponent(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
			scene_(scene) {}

		~ScriptComponent() override {}

		void onPreRender() override {
			UpdateCamera(&scene_.getGlobalComponent<Components::View3DComponent>(), CAMERA_FREE);
		}

		void onRender() override {
			DrawGrid(10, 1.0f);
		}

	private:
		Scene& scene_;
	};
}

HARMONY_REGISTER_SCRIPT(Sandbox::ScriptComponent, script)