#pragma once

namespace Harmony::Components
{
	class Transform : public sf::Transformable
	{
	public:
		Transform(const Utilities::Configuration& configuration);
		~Transform();
	};
}

