#include "pch.h"
#include "SceneNode.h"
#include <stdexcept>
#include <memory>
#include <utility>
#include "Event.h"
#include "Object.h"
#include "Circle.h"
#include "Rectangle.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace harmony::core {

    SceneNode::SceneNode(const Configuration& configuration)
        : Object(configuration), isDrawEnable(true), isUpdateEnable(true),
        parentNode(nullptr), currentScene(nullptr),
        rotationVelocity(0), rotationAcceleration(0) {
        
		if (const auto children = configuration.getData({ "Children" })) {
			for (const auto& child : children.value()) {
				attachChild(create(Configuration(child)));
			}
		}

        // Set position
        if (const auto position = configuration.getData({ "Position" })) {
            setPosition(position.value()[0], position.value()[1]);
        }

        // Set scale
        if (const auto scale = configuration.getData({ "Scale" })) {
            setScale(scale.value()[0], scale.value()[1]);
        }

        // Set rotation
        if (const auto rotation = configuration.getData<float>({ "Rotation" })) {
            setRotation(rotation.value());
        }

        // Set origin
        if (const auto origin = configuration.getData({ "Origin" })) {
            setOrigin(origin.value()[0], origin.value()[1]);
        }

        // Set position velocity
        if (const auto positionVelocity = configuration.getData({ "PositionVelocity" })) {
            this->positionVelocity = { positionVelocity.value()[0], positionVelocity.value()[1] };
        }

        // Set position acceleration
        if (const auto positionAcceleration = configuration.getData({ "PositionAcceleration" })) {
            this->positionAcceleration = { positionAcceleration.value()[0], positionAcceleration.value()[1] };
        }

        // Set rotation velocity
        if (const auto rotationVelocity = configuration.getData<float>({ "RotationVelocity" })) {
            this->rotationVelocity = rotationVelocity.value();
        }

        // Set rotation acceleration
        if (const auto rotationAcceleration = configuration.getData<float>({ "RotationAcceleration" })) {
            this->rotationAcceleration = rotationAcceleration.value();
        }

        // Set scripts
        try {
            if (const auto createScript = configuration.getData<std::string>({ "CreateScript" })) {
                this->m_createScript = CreateScript::getScript(createScript.value());
            }
            if (const auto destroyScript = configuration.getData<std::string>({ "DestroyScript" })) {
                this->m_destroyScript = DestroyScript::getScript(destroyScript.value());
            }
            if (const auto enterScript = configuration.getData<std::string>({ "EnterScript" })) {
                this->m_enterScript = EnterScript::getScript(enterScript.value());
            }
            if (const auto exitScript = configuration.getData<std::string>({ "ExitScript" })) {
                this->m_exitScript = ExitScript::getScript(exitScript.value());
            }
		} catch (const std::exception& e) {
			throw std::runtime_error("Error setting scripts: " + std::string(e.what()));
		}

        // Execute create script
        if (m_createScript) {

			int a = 0;
            m_createScript->execute(this);
        }
    }


    SceneNode::~SceneNode() {
        for (auto& child : children) {
            child->parentNode = nullptr;
        }
        children.clear();
        if (m_destroyScript){
            m_destroyScript->execute(shared_from_this());
        }
    }

    void SceneNode::attachChild(const std::shared_ptr<SceneNode> child) {
        if (child) {
            child->parentNode = this;
            child->currentScene = currentScene;
            children.push_back(child);
        }
        else {
            throw std::invalid_argument("Attempted to attach a null child to SceneNode.");
        }
    }

    std::shared_ptr<SceneNode> SceneNode::detachChild(const SceneNode& child) {
        auto found = std::find_if(children.begin(), children.end(),
            [&child](const std::shared_ptr<SceneNode>& ptr) { return ptr.get() == &child; });

        if (found != children.end()) {
            auto detachedChild = *found;
            detachedChild->parentNode = nullptr;
            detachedChild->currentScene = nullptr;
            children.erase(found);
            return detachedChild;
        }
        else {
            throw std::runtime_error("Child node not found in SceneNode.");
        }
    }

    std::shared_ptr<SceneNode> SceneNode::detachChild() {
        if (parentNode) {
            parentNode->detachChild(*this);
        }
        return std::static_pointer_cast<SceneNode>(shared_from_this());
    }

    void SceneNode::enableDraw(const bool option) {
        isDrawEnable = option;
    }

    void SceneNode::enableUpdate(const bool option) {
        isUpdateEnable = option;
    }

    sf::Transform SceneNode::getGlobalTransform() const {
        sf::Transform transform = getTransform();
        const SceneNode* currentParent = parentNode;

        while (currentParent) {
            transform *= currentParent->getTransform();
            currentParent = currentParent->parentNode;
        }
        return transform;
    }

    sf::Vector2f SceneNode::getGlobalPosition() const {
        sf::Vector2f position = getPosition();
        const SceneNode* currentParent = parentNode;

        while (currentParent) {
            position += currentParent->getPosition();
            currentParent = currentParent->parentNode;
        }
        return position;
    }

    sf::FloatRect SceneNode::getGlobalBounds() const {
        const sf::Transform globalTransform = getGlobalTransform();

        if (auto rectangle = std::dynamic_pointer_cast<sf::RectangleShape>(drawable)) {
            return globalTransform.transformRect(rectangle->getGlobalBounds());
        }

        if (auto circle = std::dynamic_pointer_cast<sf::CircleShape>(drawable)) {
            return globalTransform.transformRect(circle->getLocalBounds());
        }

        if (auto sprite = std::dynamic_pointer_cast<sf::Sprite>(drawable)) {
            return globalTransform.transformRect(sprite->getGlobalBounds());
        }

        if (auto text = std::dynamic_pointer_cast<sf::Text>(drawable)) {
            return globalTransform.transformRect(text->getGlobalBounds());
        }

        if (auto convex = std::dynamic_pointer_cast<sf::ConvexShape>(drawable)) {
            return globalTransform.transformRect(convex->getGlobalBounds());
        }

        if (auto vertexArray = std::dynamic_pointer_cast<sf::VertexArray>(drawable)) {
            if (vertexArray->getVertexCount() > 0) {
                sf::FloatRect bounds;
                for (size_t i = 0; i < vertexArray->getVertexCount(); ++i) {
                    bounds = bounds == sf::FloatRect() ? sf::FloatRect(vertexArray->operator[](i).position, { 0.f, 0.f }) : bounds;
                    bounds.left = std::min(bounds.left, vertexArray->operator[](i).position.x);
                    bounds.top = std::min(bounds.top, vertexArray->operator[](i).position.y);
                    bounds.width = std::max(bounds.width, vertexArray->operator[](i).position.x - bounds.left);
                    bounds.height = std::max(bounds.height, vertexArray->operator[](i).position.y - bounds.top);
                }
                return globalTransform.transformRect(bounds);
            }
        }

        throw std::runtime_error("No matching drawable type found.");
    }

    void SceneNode::draw(sf::RenderTarget& renderTarget, sf::RenderStates state) const {
        if (isDrawEnable) {
            state.transform *= getTransform();
            onDraw(renderTarget, state);

            for (const auto& child : children) {
                child->draw(renderTarget, state);
            }
        }
    }

    void SceneNode::update(const sf::Time& time, EventQueue& eventQueue) {
        if (isUpdateEnable) {
            onUpdate(time, eventQueue);
            updateTransform(time);

            for (const auto& child : children) {
                child->update(time, eventQueue);
            }
        }
    }

    void SceneNode::updateTransform(const sf::Time& time) {
        float deltaTime = time.asSeconds();

        move(
            positionVelocity.x * deltaTime + 0.5f * positionAcceleration.x * deltaTime * deltaTime,
            positionVelocity.y * deltaTime + 0.5f * positionAcceleration.y * deltaTime * deltaTime
        );

        rotate(rotationVelocity * deltaTime + 0.5f * rotationAcceleration * deltaTime * deltaTime);

        positionVelocity += (positionAcceleration * deltaTime);
        rotationVelocity += (rotationAcceleration * deltaTime);
    }

    void SceneNode::onDraw(sf::RenderTarget& renderTarget, sf::RenderStates state) const {
        if (drawable) {
            renderTarget.draw(*drawable, state);
        }
    }

    void SceneNode::onUpdate(const sf::Time& time, EventQueue& eventQueue) {}

    void SceneNode::onCreate(Scene& scene) {}

    void SceneNode::onDestroy(Scene& scene) {}

    void SceneNode::onEnter(Scene& scene) {
        onCreate(scene);
        for (auto& child : children) {
            child->onEnter(scene);
        }
    }

    void SceneNode::onExit(Scene& scene) {
        onDestroy(scene);
        for (auto& child : children) {
            child->onExit(scene);
        }
    }

    bool SceneNode::intersect(const std::shared_ptr<SceneNode> target) {
        return intersect(std::static_pointer_cast<SceneNode>(shared_from_this()), target);
    }

    bool SceneNode::intersect(const std::shared_ptr<SceneNode> node1, const std::shared_ptr<SceneNode> node2) {
        return node1->getGlobalBounds().intersects(node2->getGlobalBounds());
    }
    std::shared_ptr<SceneNode> SceneNode::create(const Configuration& configuration)
    {
        if (const auto type = configuration.getData<std::string>({ "Type" })) {
            if (type.value() == "Rectangle") {
                return utilities::create<Rectangle>(configuration);
            }
            if (type.value() == "Circle") {
                return utilities::create<Circle>(configuration);
            }
        }
		return utilities::create<SceneNode>(configuration);
    }
}