#include "pch.h"
#include "ModelRenderableComponent.h"
#include "ScriptComponent.h"
#include "TransformComponent.h"
#include "ModelIDComponent.h"
#include "ResourceHandler.h"
#include "Engine.h"
#include "ModelResource.h"
#include <rlgl.h> 

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
		rlPushMatrix();

		if (m_scene.containsComponent<TransformComponent>(m_entityId))
			rlMultMatrixf((float*)&m_scene.getComponent<TransformComponent>(m_entityId).getMatrix().m0);

		if (m_scene.containsComponent<ModelID>(m_entityId))
		{
			Resources::ResourceID modelResourceId = m_scene.getComponent<ModelID>(m_entityId).resourceId();
			auto resourceAccess = m_scene.engine.resourcesHandler->acquireResource(modelResourceId);

			DrawModel(resourceAccess->resource<Resources::ModelResource>().model(), {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
		}

		if (m_scene.containsComponent<ScriptComponent>(m_entityId))
			m_scene.getComponent<ScriptComponent>(m_entityId).onRender();
		
		rlPopMatrix();
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
