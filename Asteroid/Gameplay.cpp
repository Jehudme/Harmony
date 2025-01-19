#include "Gameplay.h"
#include "Player.h"
#include "AsteroidGroup.h"
#include "BackgroundStar.h"

void Asteroid::Gameplay::onEnterCurrent()
{
	for (int index = 0; index < 160; index++)
	{
		sceneGraph->attachChild(Harmony::Core::Object::create<BackgroundStar>());
	}

	sceneGraph->attachChild(Harmony::Core::Object::create<AsteroidGroup>());
	sceneGraph->attachChild(Harmony::Core::Object::create<Player>());
}

void Asteroid::Gameplay::onExitCurrent()
{

}
