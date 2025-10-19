#pragma once
#include "Logger.h"
#include <SFML/Graphics.hpp>

namespace Harmony::Components
{
	class Drawable : public sf::Drawable
	{
	public:
		Drawable() = default;
		virtual ~Drawable() = default;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			HARMONY_CRITICAL("Drawable::draw called on base class");
			throw std::runtime_error("Drawable::draw must be implemented in derived classes");
		};
	};
}

