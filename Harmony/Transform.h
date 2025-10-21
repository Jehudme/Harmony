#pragma once

#include <memory>

namespace Harmony::Components
{
	/// @brief Transform component - position, rotation, and scale
	/// Uses composition to hide SFML implementation details
	class Transform
	{
	public:
		Transform(const Utilities::Configuration& configuration);
		~Transform();

		// Position
		void setPosition(float x, float y);
		void setPosition(const float position[2]);
		void getPosition(float& x, float& y) const;

		// Rotation
		void setRotation(float angle);
		float getRotation() const;

		// Scale
		void setScale(float x, float y);
		void setScale(const float scale[2]);
		void getScale(float& x, float& y) const;

		// Origin
		void setOrigin(float x, float y);
		void setOrigin(const float origin[2]);
		void getOrigin(float& x, float& y) const;

		// Internal access for rendering - returns void* to sf::Transformable
		void* getInternalTransform();
		const void* getInternalTransform() const;

	private:
		// PImpl to hide sf::Transformable
		struct TransformImpl;
		std::unique_ptr<TransformImpl> impl_;
	};
}

