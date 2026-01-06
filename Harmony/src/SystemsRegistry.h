#include "Harmony/ISystem.h"
#include "Harmony/ISystemsRegistry.h"
#include "Harmony/Properties.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Harmony
{
class Engine;
class SystemsRegistry : public ISystemsRegistry
{
public:
    SystemsRegistry() = default;
    ~SystemsRegistry() = default;

    State GetState() const override;
    ISystem* GetSystem(const std::string role) override;
    bool ContainsSystem(const std::string role) const override;

    void Initialize(const Properties& properties);
    void Finalize();

    void CreateSystem(const std::string name);
    void DeleteSystem(const std::string role);

private:
    std::unordered_map<std::string, std::unique_ptr<ISystem>> m_systems;
    State m_state = State::Uninitialized;
};
} // namespace Harmony