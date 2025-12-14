
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

		void onUpdate() override {
			static int id;
			if (IsKeyPressed(KEY_SPACE) && !m_exist) {
				id = scene_.createEntity(101);
				m_exist = true;
				HARMONY_INFO("Car created");
			}
			else if (IsKeyPressed(KEY_LEFT_SHIFT) && m_exist)
			{
				scene_.destroyEntity(id);	
				m_exist = false;
				HARMONY_INFO("Car deleted");
			}
		}

	private:
		Scene& scene_;
		bool m_exist = false;
	};
}

HARMONY_REGISTER_SCRIPT(Sandbox::ScriptComponent, script)