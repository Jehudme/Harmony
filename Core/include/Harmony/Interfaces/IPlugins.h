#pragma once

#include "Harmony/Properties.h"

#define HARMONY_INIT_ARGS const Harmony::EngineContext& ctx, const Harmony::Properties& properties
#define HARMONY_INIT_PASS ctx, properties

namespace Harmony {
	class EngineContext;

	class IPlugins {
	public:
		virtual ~IPlugins() = default;

		virtual void Initialize(HARMONY_INIT_ARGS) = 0;

		virtual const char* GetName() const = 0;
		virtual const char* GetVersion() const = 0;
	};
}