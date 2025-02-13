#include "pch.h"
#include "Object.h"
#include "Engine.h"
#include "Rectangle.h"
#include "Script.h"
#include "TaskQueue.h"


HARMONY_REGISTER_SCRIPT(PlayerScript);

struct PlayerScript : Harmony::Script
{
	void onUpdate(std::shared_ptr<Object> target, const sf::Time& time, Harmony::TaskQueue& taskQueue) override {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			std::static_pointer_cast<Harmony::SceneNode>(target)->rotate(20 * time.asSeconds());
	}
};

int main()
{
	std::shared_ptr<Harmony::Configuration> configuration = Harmony::create<Harmony::Configuration>();

	configuration->setFilePath("Configuration.json");
	configuration->load();

	std::shared_ptr<Harmony::Engine> engine = Harmony::create<Harmony::Engine>(configuration);

	engine->run();
}