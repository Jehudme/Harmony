#include "pch.h"
#include "Engine.h"
#include "Configuration.h"

int main()
{
	Harmony::globalConfig->setFilePath("Configuration.json");
	Harmony::globalConfig->load();

	std::shared_ptr<Harmony::Engine> engine = Harmony::create<Harmony::Engine>(Harmony::globalConfig);

	engine->run();
}