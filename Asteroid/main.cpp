#include "Harmony/Engine.h"
#include "GameState.h"
int main()
{
	auto engine = harmony::utilities::onEnter<harmony::core::Engine>();

	engine->stateManager->addState("GameState", harmony::utilities::onEnter<Asteroid::GameState>());
	engine->stateManager->swichState("GameState");
	engine->initializeWindow();
	engine->run();
}