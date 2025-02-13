#include "pch.h"
#include "Object.h"
#include "Engine.h"
#include "Rectangle.h"
#include "Script.h"

HARMONY_REGISTER_SCRIPT(PlayerScript);

struct PlayerScript : Harmony::Script
{
};

int main()
{
	std::shared_ptr<Harmony::Configuration> configuration = Harmony::create<Harmony::Configuration>();

	configuration->setFilePath("Configuration.json");
	configuration->load();

	std::shared_ptr<Harmony::Engine> engine = Harmony::create<Harmony::Engine>(configuration);

	engine->run();

	auto script = Harmony::find<Harmony::Script>("PlayerScript");

	int a = 1 + 1;
}