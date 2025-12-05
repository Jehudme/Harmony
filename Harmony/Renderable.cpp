#include "pch.h"
#include "Renderable.h"
#include "Script.h"
#include "Transform.h"
#include <rlgl.h>

namespace Harmony::Components{
	Renderable::Renderable(Internals::Scene& scene, Internals::EntityID entityId) :
		entityId_(entityId),
		scene_(scene) {}

	Renderable::~Renderable() = default;

	void Renderable::preRender()
	{
		//rlPushMatrix();

		if (scene_.containsComponent<Transform>(entityId_)) {
			//rlMultMatrixf(&scene_.getComponent<Transform>(entityId_).getMatrix().m0);
		}

		if (scene_.containsComponent<Components::Script>(entityId_)) {
			scene_.getComponent<Components::Script>(entityId_).onPreRender();
		}
	}

	void Renderable::postRender()
	{
		if (scene_.containsComponent<Components::Script>(entityId_)) {
			scene_.getComponent<Components::Script>(entityId_).onPostRender();
		}

		//rlPopMatrix();
	}
}
