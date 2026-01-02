#pragma once

namespace Harmony 
{
	class ISystem {
	public:
		enum class State 
		{
			Uninitialized,
			Initialized,
		};

		virtual ~ISystem() = default;

		virtual State GetState() const = 0;
		virtual std::string GetName() const = 0;
		virtual std::string GetRole() const = 0;
		virtual std::string GetVersion() const = 0;

	private:
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
	};
} // namespace Harmony