#pragma once

namespace Harmony {
	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		virtual std::string GetName() const = 0;
		virtual std::string GetRole() const = 0;
		virtual std::string GetVersion() const = 0;

	private:
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
	};
}