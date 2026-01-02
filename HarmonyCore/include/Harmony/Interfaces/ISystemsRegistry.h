#pragma once

#include "Harmony/Interfaces/ISystem.h"

namespace Harmony
{
class ISystemsRegistry
{
public:
    virtual ~ISystemsRegistry() = default;

    virtual ISystem* GetSystem(const std::string role) = 0;
    virtual void CreateSystem(const std::string name) = 0;
    virtual void DeleteSystem(const std::string role) = 0;
    virtual bool ContainsSystem(const std::string role) const = 0;
};
} // namespace Harmony

#define HARMONY_REGISTER_SYSTEM(Concrete, Name)                                                    \
    HARMONY_REGISTER_PLUGIN(Harmony::ISystem, Concrete, Name)

#define HARMONY_CREATE_SYSTEM(Name) HARMONY_CREATE_PLUGIN(Harmony::ISystem, Name)
