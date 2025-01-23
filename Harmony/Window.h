#pragma once
#include "Object.h"

namespace harmony
{
	class Window : public core::Object
	{
	public:
		Window(const uint64_t& uniqueId = NULL);

	public:
		std::shared_ptr<sf::RenderWindow> instance;
	};
}

