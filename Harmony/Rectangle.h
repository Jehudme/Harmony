#pragma once
#include <SFML/Graphics.hpp>

namespace Harmony::Components
{
	class Rectangle : public sf::Drawable
	{
	public:
		Rectangle(const Utilities::Configuration& configuration);
		~Rectangle();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::RectangleShape shape_;
	};
}

