#include "AsteroidGroup.h"
#include "Asteroid.h"
#include <Harmony/Scene.h>
#include <Harmony/NodeEvent.h>
#include <Harmony/Utilities.h>

const uint64_t Asteroid::AsteroidGroupUniqueId = harmony::utilities::generateRandomNumber<uint64_t>();

Asteroid::AsteroidGroup::AsteroidGroup() : SceneNode(AsteroidGroupUniqueId)
{
}

void Asteroid::AsteroidGroup::onCreate(harmony::core::Scene& scene)
{
	while (children.size() < 6) {
		attachChild(harmony::utilities::create<Asteroid_t>(currentScene->view));
	}
}

void Asteroid::AsteroidGroup::onDestroy(harmony::core::Scene& scene)
{
}

void Asteroid::AsteroidGroup::onUpdate(const sf::Time& time, harmony::core::EventQueue& eventQueue)
{
	if (children.size() < 6) {
		eventQueue.addEvent(harmony::utilities::create<harmony::Event::AttachNode>(
			std::static_pointer_cast<SceneNode>(shared_from_this()),
			harmony::utilities::create<Asteroid_t>(currentScene->view)
		));
	}
}
