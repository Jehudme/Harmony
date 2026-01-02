#pragma once

#include "Harmony/Interfaces/ISystemsRegistry.h"
#include "Harmony/Properties.h"

namespace Harmony
{
class IEngine
{
public:
    enum class State
    {
        Uninitialized,
        Initialized,
        Running,
        Shutdown,
    };

public:
    virtual ~IEngine() = default;

    virtual void Initialize(const Properties& properties) = 0;

    virtual void Run() = 0;
    virtual void Shutdown() = 0;

    virtual State GetState() const = 0;
    virtual ISystemsRegistry& GetSystemsRegistry() = 0;

private:
    SystemsRegistry m_systemsRegistry;
};

} // namespace Harmony