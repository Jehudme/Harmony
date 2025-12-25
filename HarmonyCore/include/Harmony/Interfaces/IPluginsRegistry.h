#pragma once
#include "Harmony/Properties.h"
#include "Harmony/Interfaces/IPlugins.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Harmony {
	class IPluginsRegistry {
	public:
		virtual ~IPluginsRegistry() = default;

		virtual IPlugins* GetPlugin(const std::string& tag) const = 0;
		virtual IPlugins* operator[](const std::string& tag) const = 0;
		virtual bool Contains(const std::string& tag) const = 0;

	private:
		virtual void AddPlugin(const std::string& tag, const Properties& properties) = 0;
		virtual void RemovePlugin(const std::string& tag) = 0;
	};
} // namespace Harmony