#include "pch.h"
#include "Node.h"
#include "Scene.h"

namespace Harmony::Components
{
	Node::Node(EntityID entityId, Scenes::Scene& scene)
		: scene_(scene), parent_(entt::null), entityId_(entityId), children_()
	{
	}

	Node::~Node() = default;

	void Node::attach(EntityID child) {
		if (std::find(children_.begin(), children_.end(), child) == children_.end()) children_.push_back(child);
		else HARMONY_WARN("Entity {} is already a child of this node", static_cast<std::uint32_t>(child));
	
		Node& childNode = scene_.getComponent<Node>(child);
		childNode.parent_ = entityId_;
	}

	void Node::detach(EntityID child) {

		if (auto it = std::find(children_.begin(), children_.end(), child); it != children_.end()) children_.erase(it);
		else HARMONY_WARN("Entity {} is not a child of this node", static_cast<std::uint32_t>(child));
	
		scene_.getComponent<Node>(child).parent_ = entt::null;
	}

	void Node::detach() {
		if (parent_ != entt::null) scene_.getComponent<Node>(parent_).detach(entityId_);
		else HARMONY_WARN("Node has no parent to detach from");
	}


	EntityID Node::getParent() const noexcept {
		return parent_;
	}

	const std::vector<EntityID>& Node::getChildren() const noexcept {
		return children_;
	}
}
