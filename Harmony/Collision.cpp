#include "pch.h"
#include "Collision.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Logger.h"
#include <cmath>

HARMONY_REGISTER_COMPONENT(Harmony::Components::Collision, Collision)

namespace Harmony::Components
{
	struct CollisionImpl {
		Collision::Shape shape = Collision::Shape::Rectangle;
		float width = 0.0f;
		float height = 0.0f;
		float radius = 0.0f;
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		bool enabled = true;
		bool isTrigger = false;
		int layer = 0;
		int mask = 0xFFFFFFFF; // Collide with all layers by default
		Collision::CollisionCallback onEnter;
		Collision::CollisionCallback onExit;
		Collision::CollisionCallback onStay;
	};

	Collision::Collision(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<CollisionImpl>())
	{
		try {
			// Shape
			std::string shapeStr = configuration.get<std::string>({ "shape" }).value_or("rectangle");
			impl_->shape = (shapeStr == "circle") ? Shape::Circle : Shape::Rectangle;

			// Bounds
			impl_->width = configuration.get<float>({ "width" }).value_or(0.0f);
			impl_->height = configuration.get<float>({ "height" }).value_or(0.0f);
			impl_->radius = configuration.get<float>({ "radius" }).value_or(0.0f);

			// Offset
			impl_->offsetX = configuration.get<float>({ "offset", "x" }).value_or(0.0f);
			impl_->offsetY = configuration.get<float>({ "offset", "y" }).value_or(0.0f);

			// Properties
			impl_->enabled = configuration.get<bool>({ "enabled" }).value_or(true);
			impl_->isTrigger = configuration.get<bool>({ "is_trigger" }).value_or(false);
			impl_->layer = configuration.get<int>({ "layer" }).value_or(0);
			impl_->mask = configuration.get<int>({ "mask" }).value_or(0xFFFFFFFF);

			HARMONY_DEBUG("Collision component initialized with shape {}",
				impl_->shape == Shape::Rectangle ? "rectangle" : "circle");
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Collision component: {}", e.what());
			throw;
		}
	}

	Collision::~Collision() = default;

	void Collision::setShape(Shape shape) {
		impl_->shape = shape;
	}

	Collision::Shape Collision::getShape() const {
		return impl_->shape;
	}

	void Collision::setBounds(float width, float height) {
		impl_->width = width;
		impl_->height = height;
	}

	void Collision::getBounds(float& width, float& height) const {
		width = impl_->width;
		height = impl_->height;
	}

	void Collision::setRadius(float radius) {
		impl_->radius = radius;
	}

	float Collision::getRadius() const {
		return impl_->radius;
	}

	void Collision::setOffset(float x, float y) {
		impl_->offsetX = x;
		impl_->offsetY = y;
	}

	void Collision::getOffset(float& x, float& y) const {
		x = impl_->offsetX;
		y = impl_->offsetY;
	}

	void Collision::setEnabled(bool enabled) {
		impl_->enabled = enabled;
	}

	bool Collision::isEnabled() const {
		return impl_->enabled;
	}

	void Collision::setTrigger(bool isTrigger) {
		impl_->isTrigger = isTrigger;
	}

	bool Collision::isTrigger() const {
		return impl_->isTrigger;
	}

	void Collision::setLayer(int layer) {
		impl_->layer = layer;
	}

	int Collision::getLayer() const {
		return impl_->layer;
	}

	void Collision::setMask(int mask) {
		impl_->mask = mask;
	}

	int Collision::getMask() const {
		return impl_->mask;
	}

	bool Collision::checkCollision(const Collision& other, float x1, float y1, float x2, float y2) const {
		if (!impl_->enabled || !other.impl_->enabled) return false;

		// Check layer mask
		if (((1 << other.impl_->layer) & impl_->mask) == 0) return false;

		// Apply offsets
		x1 += impl_->offsetX;
		y1 += impl_->offsetY;
		x2 += other.impl_->offsetX;
		y2 += other.impl_->offsetY;

		// Rectangle vs Rectangle
		if (impl_->shape == Shape::Rectangle && other.impl_->shape == Shape::Rectangle) {
			float left1 = x1 - impl_->width * 0.5f;
			float right1 = x1 + impl_->width * 0.5f;
			float top1 = y1 - impl_->height * 0.5f;
			float bottom1 = y1 + impl_->height * 0.5f;

			float left2 = x2 - other.impl_->width * 0.5f;
			float right2 = x2 + other.impl_->width * 0.5f;
			float top2 = y2 - other.impl_->height * 0.5f;
			float bottom2 = y2 + other.impl_->height * 0.5f;

			return !(right1 < left2 || left1 > right2 || bottom1 < top2 || top1 > bottom2);
		}

		// Circle vs Circle
		if (impl_->shape == Shape::Circle && other.impl_->shape == Shape::Circle) {
			float dx = x2 - x1;
			float dy = y2 - y1;
			float distSq = dx * dx + dy * dy;
			float radiusSum = impl_->radius + other.impl_->radius;
			return distSq <= radiusSum * radiusSum;
		}

		// Circle vs Rectangle or Rectangle vs Circle
		const CollisionImpl* circle = (impl_->shape == Shape::Circle) ? impl_.get() : other.impl_.get();
		const CollisionImpl* rect = (impl_->shape == Shape::Rectangle) ? impl_.get() : other.impl_.get();
		float cx = (impl_->shape == Shape::Circle) ? x1 : x2;
		float cy = (impl_->shape == Shape::Circle) ? y1 : y2;
		float rx = (impl_->shape == Shape::Rectangle) ? x1 : x2;
		float ry = (impl_->shape == Shape::Rectangle) ? y1 : y2;

		float halfWidth = rect->width * 0.5f;
		float halfHeight = rect->height * 0.5f;
		float closestX = std::max(rx - halfWidth, std::min(cx, rx + halfWidth));
		float closestY = std::max(ry - halfHeight, std::min(cy, ry + halfHeight));

		float dx = cx - closestX;
		float dy = cy - closestY;
		float distSq = dx * dx + dy * dy;

		return distSq <= circle->radius * circle->radius;
	}

	void Collision::setOnCollisionEnter(CollisionCallback callback) {
		impl_->onEnter = callback;
	}

	void Collision::setOnCollisionExit(CollisionCallback callback) {
		impl_->onExit = callback;
	}

	void Collision::setOnCollisionStay(CollisionCallback callback) {
		impl_->onStay = callback;
	}
}
