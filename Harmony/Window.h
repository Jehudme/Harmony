#pragma once
#include "Object.h"

namespace harmony
{
	class Window : public core::Object
	{
	public:
		Window(const Configuration& configuration);

	public:
		std::shared_ptr<sf::RenderWindow> instance;
	};
}

