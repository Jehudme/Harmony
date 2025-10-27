#pragma once
#include <entt/fwd.hpp>

namespace  Harmony::Scenes {
	class Scene;
}

namespace Harmony::Utilities {
	class Configuration;
}


namespace Harmony::Components
{
	class Node
	{
	public:
		friend class Harmony::Scenes::Scene;
		Node(EntityID entityId, Scenes::Scene& scene);
		~Node();

		void attach(EntityID child);
		void detach(EntityID child);
		void detach();

		EntityID getParent() const noexcept;
		const std::vector<EntityID>& getChildren() const noexcept;

	private:
		Scenes::Scene& scene_;
		EntityID parent_;
		EntityID entityId_;
		std::vector<EntityID> children_;

	};

}
