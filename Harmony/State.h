#pragma once
#include "Object.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

namespace Harmony
{
	class TaskQueue;
	class State : public Object, public sf::Drawable
	{
	public:
		State(std::shared_ptr<Configuration> configuration);

		void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;
		void update(const sf::Time& time, TaskQueue& taskQueue);
	};
}

