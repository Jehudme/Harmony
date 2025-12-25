#pragma once

#include "Harmony/Properties.h"
#include "Harmony/Context.h"

#define HARMONY_PLUGIN_INIT_ARGS const Harmony::Context& context, const Harmony::Properties& properties
#define HARMONY_PLUGIN_INIT_ARG_NAMES context, properties

namespace Harmony {

	class IPlugins {
	public:
		virtual ~IPlugins() = default;

		virtual const char* GetName() const = 0;
		virtual const char* GetVersion() const = 0;
	};
}