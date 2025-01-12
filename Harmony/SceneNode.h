#pragma once

#include "Object.h"
#include "Configuration.h"
#include "Logger.h"
#include <vector>
#include <functional>
#include <memory>

namespace Harmony {

    class SceneNode : public Object, sf::Transformable {
    public:
        SceneNode();
          SceneNode(const uint64_t& configurationId);
          SceneNode(const std::shared_ptr<Configuration>& configuration);

        void attachChild(std::shared_ptr<SceneNode> sceneNode);
        void detachChild(std::shared_ptr<SceneNode> sceneNode);

        sf::Vector2f getGlobalPosition();

        template<typename... Args>
        void execute(const std::function<void(SceneNode&, Args...)>& func, Args&&... args);

        static void addChildConfigurationID(std::shared_ptr<Configuration> configuration_parent, std::shared_ptr<Configuration> configuration_child);
        static void rmvChildConfigurationID(std::shared_ptr<Configuration> configuration_parent, std::shared_ptr<Configuration> configuration_child);

    private:
        void initialize(const Configuration& configuration); // Implementation left for you

    protected:
        std::vector<std::shared_ptr<SceneNode>> children_;
        SceneNode* parent_ = nullptr;
    };

} // namespace Harmony


