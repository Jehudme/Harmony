#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

#include "Configuration.h"
#include "TaskManagement.h"

namespace Harmony::Internals
{
	class cene : public sf::Drawable {
	public:
		Scene(const Configuration& configuration);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void update(const sf::Time deltaTime, TaskQueue& taskManagement);
	};
}