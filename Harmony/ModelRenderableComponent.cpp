#include "pch.h"
#include "ModelRenderableComponent.h"
#include "ScriptComponent.h"
#include "TransformComponent.h"
#include "ModelIDComponent.h"
#include "ResourceHandler.h"
#include "Engine.h"
#include "ModelResource.h"

HARMONY_REGISTER_COMPONENT_WITH_BASE(Harmony::Components::IRenderableComponent, Harmony::Components::ModelRenderableComponent, modelRenderable);

namespace Harmony::Components
{
	ModelRenderableComponent::ModelRenderableComponent(HARMONY_COMPONENTS_CONSTRUCTOR_ARGUMENTS) :
		IRenderableComponent(HARMONY_COMPONENTS_CONSTRUCTOR_PARAMETER_NAMES)
	{
	}

	ModelRenderableComponent::~ModelRenderableComponent() = default;

	void ModelRenderableComponent::onRender()
	{
		Matrix transform = MatrixIdentity();

		if (m_scene.containsComponent<TransformComponent>(m_entityId))
			transform = m_scene.getComponent<TransformComponent>(m_entityId).getMatrix();

		if (m_scene.containsComponent<ModelID>(m_entityId))
		{
			Resources::ResourceID modelResourceId = m_scene.getComponent<ModelID>(m_entityId).resourceId();
			auto resourceAccess = m_scene.engine.resourcesHandler->acquireResource(modelResourceId);

			R3D_DrawModelPro(&resourceAccess->resource<Resources::ModelResource>().model(), transform);
		}

		if (m_scene.containsComponent<ScriptComponent>(m_entityId))
			m_scene.getComponent<ScriptComponent>(m_entityId).onRender();
	}

	void ModelRenderableComponent::onPreRender()
	{
		if (m_scene.containsComponent<ScriptComponent>(m_entityId))
			m_scene.getComponent<ScriptComponent>(m_entityId).onPreRender();
	}

	void ModelRenderableComponent::onPostRender()
	{
		if (m_scene.containsComponent<ScriptComponent>(m_entityId))
			m_scene.getComponent<ScriptComponent>(m_entityId).onPostRender();
	}
}
