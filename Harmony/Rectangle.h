#pragma once
#include "SceneNode.h"
#include <SFML/Graphics/RectangleShape.hpp>

namespace Harmony
{

	class Rectangle : public SceneNode
	{
	public:
		Rectangle(std::shared_ptr<Configuration> configuration);

	private:
		virtual void drawCurrent(sf::RenderTarget& renderTarget, sf::RenderStates states) const override;

	public:
		sf::RectangleShape sprite;
	};
}

class Rectangle
{
};
