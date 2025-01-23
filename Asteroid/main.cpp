#include "Harmony/Engine.h"
#include "GameState.h"
int main()
{
	auto engine = harmony::utilities::create<harmony::core::Engine>();

	engine->stateManager->addState("GameState", harmony::utilities::create<Asteroid::GameState>());
	engine->stateManager->pushState("GameState");
	engine->initializeWindow();
	engine->run();
}