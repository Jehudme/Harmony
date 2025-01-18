#include "Gameplay.h"
#include "Player.h"

void Asteroid::Gameplay::onEnterCurrent()
{
	sceneGraph->attachChild(Harmony::Core::Object::create<Player>());
}

void Asteroid::Gameplay::onExitCurrent()
{

}
