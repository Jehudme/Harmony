#include "pch.h"
#include "Renderable.h"
#include "Script.h"
#include "Transform.h"
#include "Logger.h"
#include "Assert.h"
#include "Exceptions.h"
#include <rlgl.h>

namespace Harmony::Components{
	Renderable::Renderable(Internals::Scene& scene, Internals::EntityID entityId) :
		entityId_(entityId),
		scene_(scene) 
	{
		HARMONY_ASSERT_NOT_NULL(&scene != nullptr, "Scene reference cannot be null");
		HARMONY_TRACE("Renderable component created for entity {}", static_cast<uint32_t>(entityId));
	}

	Renderable::~Renderable() 
	{
		HARMONY_TRACE("Renderable component destroyed for entity {}", static_cast<uint32_t>(entityId_));
	}

	void Renderable::preRender()
	{
		try {
			//rlPushMatrix();

			if (scene_.containsComponent<Transform>(entityId_)) {
				//rlMultMatrixf(&scene_.getComponent<Transform>(entityId_).getMatrix().m0);
			}

			if (scene_.containsComponent<Components::Script>(entityId_)) {
				scene_.getComponent<Components::Script>(entityId_).onPreRender();
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception in Renderable::preRender for entity {}: {}", 
				static_cast<uint32_t>(entityId_), e.what());
		}
	}

	void Renderable::postRender()
	{
		try {
			if (scene_.containsComponent<Components::Script>(entityId_)) {
				scene_.getComponent<Components::Script>(entityId_).onPostRender();
			}

			//rlPopMatrix();
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Exception in Renderable::postRender for entity {}: {}", 
				static_cast<uint32_t>(entityId_), e.what());
		}
	}
}
