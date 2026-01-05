#include "Engine.h"
#include "Harmony/Logger.h"
#include "Harmony/Properties.h"
#include <Harmony/Interfaces/ISystemsRegistry.h>


class TestSystem : public Harmony::ISystem
{
    public:
    TestSystem() = default;
    ~TestSystem() override = default;
    State GetState() const override
    {
        return State::Initialized;
    }
    std::string GetName() const override
    {
        return "Test";
    }
    std::string GetRole() const override
    {
        return "Test";
    }
    std::string GetVersion() const override
    {
        return "1.0.0";
    }
    void Initialize(const Harmony::Properties& properties) override
    {
        Harmony::Logger::Info("TestSystem initialized.");
    }
    void Finalize() override
    {
        Harmony::Logger::Info("TestSystem finalized.");
    }
};

HARMONY_REGISTER_SYSTEM(TestSystem, "Test");

int main()
{
    Harmony::Logger::Initialize("harmony.log");

    Harmony::Engine engine;
    Harmony::Properties properties;

    properties.Load("assets/properties.json");

    engine.Initialize(properties);
    engine.Run();
    return 0;
}