#include "AsteroidGroup.h"
#include <SFML/System/Vector2.hpp>
#include "Asteroid.h"
#include <Harmony/Scene.h>
#include <Harmony/NodeEvent.h>

const uint64_t Asteroid::AsteroidGroupUniqueId = Harmony::Utilities::generateRandomNumber<uint64_t>();

Asteroid::AsteroidGroup::AsteroidGroup() : SceneNode(AsteroidGroupUniqueId)
{
}

void Asteroid::AsteroidGroup::onEnterCurrent(Harmony::Core::Scene& scene)
{
	while (children.size() < 6) {
		attachChild(Harmony::Core::Object::create<Asteroid_t>(currentScene->view));
	}
}

void Asteroid::AsteroidGroup::onExitCurrent(Harmony::Core::Scene& scene)
{
}

void Asteroid::AsteroidGroup::updateCurrent(const sf::Time& time, Harmony::Core::EventPool& eventPool)
{
	if (children.size() < 6) {
		eventPool.addEvent(Harmony::Core::Object::create<Harmony::Event::AttachNode>(
			std::static_pointer_cast<SceneNode>(shared_from_this()),
			Harmony::Core::Object::create<Asteroid_t>(currentScene->view)
		));
	}
}
