#pragma once

#include <memory>
#include <mutex>
#include <string>
#include "Configuration.h"

namespace Harmony::Internals {
	class Engine;
}

namespace Harmony::Internals {

	/// Scene - Represents a game scene with its own data and logic
	/// A scene contains entities, components, and game-specific logic
	/// Thread-safe design for concurrent access
	class Scene {
	public:
		/// Constructor
		/// @param engine Reference to the engine
		/// @param configuration Configuration for the scene
		/// @param name Optional name for the scene
		Scene(Engine& engine, const Configuration& configuration, const std::string& name = "UnnamedScene");

		/// Virtual destructor
		virtual ~Scene();

		// Prevent copying
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		// Allow moving
		Scene(Scene&&) noexcept = default;
		Scene& operator=(Scene&&) noexcept = default;

		/// Initialize the scene
		virtual void initialize();

		/// Cleanup the scene
		virtual void cleanup();

		/// Update the scene
		/// @param deltaTime Time elapsed since last handleResources
		virtual void handleResources(float deltaTime);

		/// Render the scene
		virtual void render();

		/// Handle events for the scene
		virtual void handleEvents();

		/// Get the scene name
		const std::string& getName() const;

		/// Check if the scene is active
		bool isActive() const;

		/// Set the active status
		void setActive(bool active);

		/// Get the draw order (lower values are drawn first)
		int getDrawOrder() const;

		/// Set the draw order
		void setDrawOrder(int order);

	protected:
		/// Get the engine reference
		Engine& getEngine();

		/// Get the configuration reference
		const Configuration& getConfiguration() const;

	private:
		mutable std::mutex mutex_;
		Engine& engine_;
		Configuration configuration_;
		std::string name_;
		bool active_;
		int drawOrder_;
	};

} // namespace Harmony::Internals
