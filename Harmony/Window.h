#pragma once

namespace Harmony {
	class Window : public sf::RenderWindow
	{
		Window(const Utilities::Configuration& configuration);
		~Window();
	};
}

