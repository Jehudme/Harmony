#include "pch.h"
#include "Harmony.h"

namespace Harmony::Core {

    // Object
    void Object::retainSelf() {
        m_retainedObjects[this] = shared_from_this();
    }

    void Object::releaseSelf() {
        if (m_retainedObjects.contains(this)) {
            m_retainedObjects.erase(this);
        }
    }

    Object::Object(const uint64_t& uniqueId) : uniqueId(uniqueId) {}

    // Utilities::Random
    uint64_t Utilities::Random::generateId() {
        static std::random_device rd;
        static std::mt19937_64 engine(rd());
        static std::uniform_int_distribution<uint64_t> dist;
        return dist(engine);
    }

    // Scene
    Scene::Scene(const uint64_t& uniqueId)
        : Object(uniqueId),
        m_rootNode(std::make_shared<SceneNode>()),
        m_view() {
    }

    void Scene::update(const sf::Time& time) {
        if (m_rootNode) {
            m_rootNode->update(time);
        }
    }

    void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (m_rootNode) {
            target.setView(m_view);
            m_rootNode->draw(target, states);
        }
    }

    // SceneNode
    SceneNode::SceneNode(const uint64_t& uniqueId)
        : Object(uniqueId), m_parent(nullptr), m_isDrawEnabled(true), m_isUpdateEnabled(true) {
    }

    void SceneNode::attachChild(std::shared_ptr<SceneNode> sceneNode) {
        if (sceneNode) {
            sceneNode->m_parent = this;
            m_children.push_back(std::move(sceneNode));
        }
    }

    void SceneNode::detachChild(std::shared_ptr<SceneNode> sceneNode) {
        if (!sceneNode) return;
        auto it = std::find(m_children.begin(), m_children.end(), sceneNode);
        if (it != m_children.end()) {
            (*it)->m_parent = nullptr;
            m_children.erase(it);
        }
    }

    sf::Vector2f SceneNode::getGlobalPosition() {
        sf::Transform transform = getTransform();
        if (m_parent) {
            transform = m_parent->getTransform() * transform;
        }
        return transform.transformPoint(0.f, 0.f);
    }

    template<typename... Args>
    void SceneNode::execute(const std::function<void(SceneNode&, Args...)>& func, Args&&... args) {
        if (func) {
            func(*this, std::forward<Args>(args)...);
        }
    }

    void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        if (m_isDrawEnabled) {
            drawCurrent(target, states);
            for (const auto& child : m_children) {
                child->draw(target, states);
            }
        }
    }

    void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {}

    void SceneNode::update(const sf::Time& time) {
        if (m_isUpdateEnabled) {
            updateCurrent(time);
            for (auto& child : m_children) {
                child->update(time);
            }
        }
    }

    void SceneNode::updateCurrent(const sf::Time& time) {}

    // State
    State::State(const uint64_t& uniqueId) : Object(uniqueId) {}

    void State::update(const sf::Time& time) {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->update(time);
        }
    }

    void State::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.top()->draw(target, states);
        }
    }

    void State::onEnter() {}
    void State::onExit() {}

    void State::addScene(const std::string& sceneName, std::shared_ptr<Scene> scene) {
        scenes[sceneName] = scene;
    }

    std::shared_ptr<Scene> State::getScene(const std::string& sceneName) {
        if (scenes.find(sceneName) != scenes.end()) {
            return scenes.at(sceneName);
        }
        return nullptr;
    }

    void State::switchToScene(const std::string& sceneName) {
        auto scene = getScene(sceneName);
        if (scene) {
            m_renderBuffer.push(scene);
        }
    }

    void State::popScene() {
        if (!m_renderBuffer.empty()) {
            m_renderBuffer.pop();
        }
    }

    bool State::isSceneActive(const std::string& sceneName) const {
        return scenes.find(sceneName) != scenes.end();
    }

    // StateManager
    StateManager::StateManager(const uint64_t& uniqueId) : Object(uniqueId)
    {
    };


    void StateManager::pushState(std::shared_ptr<State> state) {
        if (!m_stateStack.empty()) {
            m_stateStack.top()->onExit();
        }
        m_stateStack.push(state);
        state->onEnter();
    }

    void StateManager::popState() {
        if (!m_stateStack.empty()) {
            m_stateStack.top()->onExit();
            m_stateStack.pop();
            if (!m_stateStack.empty()) {
                m_stateStack.top()->onEnter();
            }
        }
    }

    std::shared_ptr<State> StateManager::getCurrentState() const {
        return m_stateStack.empty() ? nullptr : m_stateStack.top();
    }

    void StateManager::update(const sf::Time& time) {
        if (!m_stateStack.empty()) {
            m_stateStack.top()->update(time);
        }
    }

    void StateManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!m_stateStack.empty()) {
            m_stateStack.top()->draw(target, states);
        }
    }

    bool StateManager::hasStates() const {
        return !m_stateStack.empty();
    }

    Engine::Engine(const uint64_t& uniqueId) : Object(uniqueId), stateManager(Object::create<StateManager>()) {
    }

    void Engine::run() {
        while (renderWindow.isOpen()) {
            event();
            update();
            draw();
        }
    }

    void Engine::update() {
        stateManager->update(clock.restart());
    }

    void Engine::event() {
        sf::Event event;
        while (renderWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                renderWindow.close();
        }
    }

    void Engine::draw() {
        renderWindow.clear();
        renderWindow.draw(*stateManager);
        renderWindow.display();
    }

} // namespace Harmony::Core
