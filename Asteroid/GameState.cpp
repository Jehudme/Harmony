#include "GameState.h"
#include "Gameplay.h"

void Asteroid::GameState::create()
{
	this->addScene("Gameplay", harmony::utilities::create<Gameplay>());
	this->switchToScene("Gameplay");
}

void Asteroid::GameState::destroy()
{
}
