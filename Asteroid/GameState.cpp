#include "GameState.h"
#include "Gameplay.h"

void Asteroid::GameState::onEnter()
{
	this->addScene("Gameplay", harmony::utilities::create<Gameplay>());
	this->pushScene("Gameplay");
}

void Asteroid::GameState::onExit()
{
}
