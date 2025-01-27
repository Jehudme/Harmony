#pragma once
#include "SceneNode.h"
#include "Configuration.h"

namespace harmony
{
	class Rectangle : public core::SceneNode
	{
	public:
		Rectangle(const Configuration& configuration);

	public:
		std::shared_ptr<sf::RectangleShape> rectangle;
	};
}
