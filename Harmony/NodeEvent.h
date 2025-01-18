#pragma once
#include "Event.h"
#include "SceneNode.h"

namespace Harmony::Event
{
	class AttachNode : public Harmony::Core::Event_t
	{
	public:
		AttachNode(
			const std::shared_ptr<Core::SceneNode> parentNode,
			const std::shared_ptr<Core::SceneNode> childNode) : 
			Core::Event_t("Attach_Node"), m_parentNode(parentNode), m_childNode(childNode){}

		inline void execute() override {
			m_parentNode->attachChild(m_childNode);
		}
	
	private:
		std::shared_ptr<Core::SceneNode> m_parentNode;
		std::shared_ptr<Core::SceneNode> m_childNode;
	};

	class DetachNode : public Harmony::Core::Event_t
	{
	public:
		DetachNode(const std::shared_ptr<Core::SceneNode> node) :
			Core::Event_t("Detach_Node"), m_node(node) {}

		inline void execute() override {
			m_node->detachChild();
		}

	private:
		std::shared_ptr<Core::SceneNode> m_node;
	};
}

