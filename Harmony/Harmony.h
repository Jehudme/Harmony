#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <functional>
#include <stack>
#include <map>
#include <string>
#include <cstdint>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics.hpp>

namespace Harmony::Core {

    // Utilities::Random
    namespace Utilities {
        class Random {
        public:
            static uint64_t generateId();
        };
    }

    // Object
    class Object : public std::enable_shared_from_this<Object>, public sf::NonCopyable {
    public:
        template<typename Type, typename... ARGS>
        static std::shared_ptr<Type> create(ARGS&&... args);

        template<typename Type>
        static std::shared_ptr<Type> find(const uint64_t& targetId);

        void retainSelf();
        void releaseSelf();

        virtual ~Object() = default;

        Object(const uint64_t& uniqueId = Utilities::Random::generateId());

        const uint64_t uniqueId;
        std::string type;

    private:
        static inline std::unordered_map<uint64_t, std::weak_ptr<Object>> m_objectRegistry;
        static inline std::unordered_map<Object*, std::shared_ptr<Object>> m_retainedObjects;
    };

    template<typename Type, typename... ARGS>
    inline std::shared_ptr<Type> Object::create(ARGS&&... args) {
        static_assert(std::is_base_of_v<Object, Type>, "Type must inherit from Harmony::Core::Object");
        auto object = std::make_shared<Type>(std::forward<ARGS>(args)...);
        m_objectRegistry[object->uniqueId] = object->weak_from_this();
        object->type = typeid(Type).name();
        return object;
    }

    template<typename Type>
    inline std::shared_ptr<Type> Object::find(const uint64_t& targetId) {
        static_assert(std::is_base_of_v<Object, Type>, "Type must inherit from Harmony::Core::Object");
        if (!m_objectRegistry.contains(targetId)) {
            throw std::runtime_error("Object not found");
        }
        auto object = m_objectRegistry[targetId].lock();
        return std::dynamic_pointer_cast<Type>(object);
    }

    // Scene
    class SceneNode;

    class Scene : public Object, public sf::Drawable {
    public:
        Scene(const uint64_t& uniqueId = Utilities::Random::generateId());

        virtual void update(const sf::Time& time);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    public:
        std::shared_ptr<SceneNode> m_rootNode;
        sf::View m_view;
    };

    // SceneNode
    class SceneNode : public Object, public sf::Transformable, public sf::Drawable {
    public:
        friend Scene;

        SceneNode(const uint64_t& uniqueId = Utilities::Random::generateId());

        void attachChild(std::shared_ptr<SceneNode> sceneNode);
        void detachChild(std::shared_ptr<SceneNode> sceneNode);
        sf::Vector2f getGlobalPosition();

        template<typename... Args>
        void execute(const std::function<void(SceneNode&, Args...)>& func, Args&&... args);

    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void update(const sf::Time& time);
        virtual void updateCurrent(const sf::Time& time);

    public:
        std::vector<std::shared_ptr<SceneNode>> m_children;
        SceneNode* m_parent = nullptr;
        bool m_isDrawEnabled;
        bool m_isUpdateEnabled;
    };

    // State
    class State : public Object, public sf::Drawable {
    public:
        State(const uint64_t& uniqueId = Utilities::Random::generateId());

        virtual void update(const sf::Time& time);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void onEnter();
        virtual void onExit();

        void addScene(const std::string& sceneName, std::shared_ptr<Scene> scene);
        std::shared_ptr<Scene> getScene(const std::string& sceneName);
        void switchToScene(const std::string& sceneName);
        void popScene();
        bool isSceneActive(const std::string& sceneName) const;

    protected:
        std::map<std::string, std::shared_ptr<Scene>> scenes;
        std::stack<std::shared_ptr<Scene>> m_renderBuffer;
    };

    // StateManager
    class StateManager : public Object, public sf::Drawable {
    public:
        StateManager(const uint64_t& uniqueId = Utilities::Random::generateId());

        void pushState(std::shared_ptr<State> state);
        void popState();
        std::shared_ptr<State> getCurrentState() const;
        void update(const sf::Time& time);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        bool hasStates() const;

    private:
        std::stack<std::shared_ptr<State>> m_stateStack;
    };

    // Engine
    class Engine : public Object {
    public:
        Engine(const uint64_t& uniqueId = Utilities::Random::generateId());

        void run();

    private:
        virtual void update();
        virtual void event();
        void draw();

    public:
        std::shared_ptr<StateManager> stateManager;
        sf::RenderWindow renderWindow;
        sf::Clock clock;
    };

} // namespace Harmony::Core
