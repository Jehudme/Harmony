#pragma once

#include <memory>
#include <functional>

namespace Harmony::Components
{
	/// @brief Collision component - collision detection and response
	class Collision
	{
	public:
		enum class Shape { Rectangle, Circle };

		Collision(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Collision();

		// Shape and bounds
		void setShape(Shape shape);
		Shape getShape() const;
		void setBounds(float width, float height);
		void getBounds(float& width, float& height) const;
		void setRadius(float radius);
		float getRadius() const;

		// Offset from entity position
		void setOffset(float x, float y);
		void getOffset(float& x, float& y) const;

		// Collision properties
		void setEnabled(bool enabled);
		bool isEnabled() const;
		void setTrigger(bool isTrigger);
		bool isTrigger() const;

		// Layer and mask for selective collision
		void setLayer(int layer);
		int getLayer() const;
		void setMask(int mask);
		int getMask() const;

		// Collision check
		bool checkCollision(const Collision& other, float x1, float y1, float x2, float y2) const;

		// Callbacks
		using CollisionCallback = std::function<void(EntityID otherEntity)>;
		void setOnCollisionEnter(CollisionCallback callback);
		void setOnCollisionExit(CollisionCallback callback);
		void setOnCollisionStay(CollisionCallback callback);

	private:
		struct CollisionImpl;
		std::unique_ptr<CollisionImpl> impl_;
	};
}
