#include "Gameplay.h"
#include "Player.h"
#include "AsteroidGroup.h"
#include "BackgroundStar.h"

void Asteroid::Gameplay::onCreate()
{
	for (int index = 0; index < 1000; index++)
	{
		sceneGraph->attachChild(harmony::utilities::create<BackgroundStar>());
	}

	sceneGraph->attachChild(harmony::utilities::create<AsteroidGroup>());
	sceneGraph->attachChild(harmony::utilities::create<Player>());
}

void Asteroid::Gameplay::onDestroy()
{

}
