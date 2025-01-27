#pragma once
#include "Event.h"
#include "SceneNode.h"

namespace harmony::Event
{
	class AttachNode : public harmony::core::Event_t
	{
	public:
		AttachNode(
			const std::shared_ptr<core::SceneNode> parentNode,
			const std::shared_ptr<core::SceneNode> childNode) : 
			core::Event_t("Attach_Node"), m_parentNode(parentNode), m_childNode(childNode){}

		inline void execute() override {
			if (m_parentNode && m_childNode)
				m_parentNode->attachChild(m_childNode);
		}
	
	private:
		std::shared_ptr<core::SceneNode> m_parentNode;
		std::shared_ptr<core::SceneNode> m_childNode;
	};

	class DetachNode : public harmony::core::Event_t
	{
	public:
		DetachNode(const std::shared_ptr<core::SceneNode> node) :
			core::Event_t("Detach_Node"), m_node(node) {}

		inline void execute() override {
			if(m_node)
				m_node->detachChild();
		}

	private:
		std::shared_ptr<core::SceneNode> m_node;
	};
}

