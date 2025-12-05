
#include <Harmony/Engine.h>
#include <Harmony/Logger.h>
#include <Harmony/Script.h>
#include <Harmony/Scene.h>
#include <raylib.h>
#include <raymath.h>
#include <Harmony/View3D.h>

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
	class Script : public Harmony::Components::Script {
	public:
		Script(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
			scene_(scene) {}

		~Script() override {}

		void onPreRender() override {
			UpdateCamera(&scene_.getGlobalComponent<Components::View3D>(), CAMERA_FREE);
		}

	private:
		Scene& scene_;
	};
}

HARMONY_REGISTER_SCRIPT(Sandbox::Script, script)