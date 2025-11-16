#pragma once

#include <box2d/box2d.h>
#include <memory>
#include <vector>
#include <functional>

namespace Harmony::Scenes {
	class Scene;
}

namespace Harmony::Components
{
	/// @brief PhysicsWorld component - wraps Box2D b2World for physics simulation
	/// Uses RAII to manage world lifetime
	class PhysicsWorld
	{
	public:
		PhysicsWorld(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~PhysicsWorld();

		// Non-copyable
		PhysicsWorld(const PhysicsWorld&) = delete;
		PhysicsWorld& operator=(const PhysicsWorld&) = delete;

		// Movable
		PhysicsWorld(PhysicsWorld&&) noexcept;
		PhysicsWorld& operator=(PhysicsWorld&&) noexcept;

		/// @brief Advance the physics simulation
		/// @param timeStep Time step in seconds
		/// @param velocityIterations Number of velocity solver iterations
		/// @param positionIterations Number of position solver iterations
		void step(float timeStep, int32_t velocityIterations = 6, int32_t positionIterations = 2);

		/// @brief Create a physics body in this world
		/// @param def Body definition
		/// @return Pointer to the created body
		b2Body* createBody(const b2BodyDef* def);

		/// @brief Destroy a physics body
		/// @param body Body to destroy
		void destroyBody(b2Body* body);

		/// @brief Set world gravity
		/// @param gravity Gravity vector
		void setGravity(const b2Vec2& gravity);

		/// @brief Get world gravity
		/// @return Gravity vector
		b2Vec2 getGravity() const;

		/// @brief Query the world for all fixtures that potentially overlap the provided AABB
		/// @param callback Callback function that receives each fixture
		/// @param aabb Area to query
		void queryAABB(std::function<bool(b2Fixture*)> callback, const b2AABB& aabb);

		/// @brief Ray-cast the world for all fixtures in the path of the ray
		/// @param callback Callback function that receives each fixture
		/// @param point1 Ray starting point
		/// @param point2 Ray ending point
		void rayCast(std::function<float(b2Fixture*, const b2Vec2&, const b2Vec2&, float)> callback, 
			const b2Vec2& point1, const b2Vec2& point2);

		/// @brief Get the number of bodies in the world
		/// @return Body count
		int32_t getBodyCount() const;

		/// @brief Set contact listener for collision callbacks
		/// @param listener Contact listener (ownership not transferred)
		void setContactListener(b2ContactListener* listener);

		/// @brief Get raw world pointer
		/// @return Pointer to the underlying b2World
		b2World* get() { return world_.get(); }
		const b2World* get() const { return world_.get(); }

	private:
		// Custom deleter for b2World
		struct WorldDeleter {
			void operator()(b2World* world) const {
				delete world;
			}
		};

		std::unique_ptr<b2World, WorldDeleter> world_;
	};
}
