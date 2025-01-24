#include "GameState.h"
#include "Gameplay.h"

void Asteroid::GameState::onEnter()
{
	this->addScene("Gameplay", harmony::utilities::onEnter<Gameplay>());
	this->pushScene("Gameplay");
}

void Asteroid::GameState::onExit()
{
}
