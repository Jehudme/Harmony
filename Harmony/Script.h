#pragma once
#include "ComponentManagement.h"
#include "entt/entity/fwd.hpp"
#include "fwd.h"

namespace Harmony::Components
{
	class Script
	{
	public:
		friend class Harmony::Scenes::Scene;

		Script();
		Script(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Script();

		virtual void onCreate();
		virtual void onDestroy();

		virtual void onPreUpdate();
		virtual void onPostUpdate();

		Scenes::Scene& getScene();
		const EntityID getEntityId() const;

	private:
		EntityID entityId_;
		std::optional<std::reference_wrapper<Scenes::Scene>> scene_;
	};
}

// Simplified automatic component registration macro
#define HARMONY_REGISTER_SCRIPT(ComponentType, ComponentName)													\
    HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::Script, ComponentType, ComponentName)