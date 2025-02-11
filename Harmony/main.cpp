#include "pch.h"
#include "Object.h"
#include "Engine.h"

int main()
{
	std::shared_ptr<Harmony::Configuration> configuration = Harmony::create<Harmony::Configuration>();
	
	configuration->setFilePath("Configuration.json");
	configuration->load();

	std::shared_ptr<Harmony::Engine> engine = Harmony::create<Harmony::Engine>(configuration);

	engine->run();
}