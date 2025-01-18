#include "GameState.h"
#include "Gameplay.h"

void Asteroid::GameState::onEnter()
{
	this->addScene("Gameplay", Harmony::Core::Object::create<Gameplay>());
	this->switchToScene("Gameplay");
}

void Asteroid::GameState::onExit()
{
}
