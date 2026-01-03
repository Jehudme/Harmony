#pragma once
#include <string>
#include <Harmony/Properties.h>

namespace Harmony
{
class ISystem
{
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

    virtual void Initialize(const Properties& properties) = 0;
    virtual void Finalize() = 0;
};
} // namespace Harmony