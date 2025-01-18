#include "Harmony/Engine.h"
#include "GameState.h"

int main()
{
	auto engine = Harmony::Core::Object::create<Harmony::Core::Engine>();

	engine->stateManager->addState("GameState", Harmony::Core::Object::create<Asteroid::GameState>());
	engine->stateManager->pushState("GameState");

	engine->run();
}