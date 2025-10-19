#pragma once
#include "Drawable.h"
namespace Harmony::Components
{
	class Rectangle : public Harmony::Components::Drawable
	{
	public:
		Rectangle(const Utilities::Configuration& configuration);
		~Rectangle();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::RectangleShape shape_;
	};
}

