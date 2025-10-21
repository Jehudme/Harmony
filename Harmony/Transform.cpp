#include "pch.h"
#include "Transform.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Logger.h"
#include <SFML/Graphics.hpp>


HARMONY_REGISTER_COMPONENT(Harmony::Components::Transform, Transform)

namespace Harmony::Components
{
	// PImpl to hide sf::Transformable
	struct Transform::TransformImpl {
		sf::Transformable transformable;
	};

	Transform::Transform(const Utilities::Configuration& configuration)
		: impl_(std::make_unique<TransformImpl>())
	{
		float x = 0.0f, y = 0.0f;
		
		// Position
		if(std::optional<float> xposition = configuration.get<float>({ "position", "x" })) 
			x = xposition.value();
		else HARMONY_WARN("Transform component missing position x configuration");

		if (std::optional<float> yposition = configuration.get<float>({ "position", "y" })) 
			y = yposition.value();
		else HARMONY_WARN("Transform component missing position y configuration");
		
		impl_->transformable.setPosition(x, y);
		
		// Rotation
		if (std::optional<float> rotation = configuration.get<float>({ "rotation" }))
			impl_->transformable.setRotation(rotation.value());
		else HARMONY_WARN("Transform component missing rotation configuration");
		
		// Scale
		x = 1.0f; y = 1.0f;
		if (std::optional<float> xscale = configuration.get<float>({ "scale", "x" }))
			x = xscale.value();
		else HARMONY_WARN("Transform component missing scale x configuration");
		
		if (std::optional<float> yscale = configuration.get<float>({ "scale", "y" }))
			y = yscale.value();
		else HARMONY_WARN("Transform component missing scale y configuration");
		
		impl_->transformable.setScale(x, y);
		
		// Origin
		x = 0.0f; y = 0.0f;
		if(std::optional<float> originX = configuration.get<float>({ "origin", "x" }))
			x = originX.value();
		else HARMONY_WARN("Transform component missing origin x configuration");
		
		if (std::optional<float> originY = configuration.get<float>({ "origin", "y" }))
			y = originY.value();
		else HARMONY_WARN("Transform component missing origin y configuration");
		
		impl_->transformable.setOrigin(x, y);
	}

	Transform::~Transform() = default;

	void Transform::setPosition(float x, float y) {
		impl_->transformable.setPosition(x, y);
	}

	void Transform::setPosition(const float position[2]) {
		impl_->transformable.setPosition(position[0], position[1]);
	}

	void Transform::getPosition(float& x, float& y) const {
		const auto& pos = impl_->transformable.getPosition();
		x = pos.x;
		y = pos.y;
	}

	void Transform::setRotation(float angle) {
		impl_->transformable.setRotation(angle);
	}

	float Transform::getRotation() const {
		return impl_->transformable.getRotation();
	}

	void Transform::setScale(float x, float y) {
		impl_->transformable.setScale(x, y);
	}

	void Transform::setScale(const float scale[2]) {
		impl_->transformable.setScale(scale[0], scale[1]);
	}

	void Transform::getScale(float& x, float& y) const {
		const auto& scale = impl_->transformable.getScale();
		x = scale.x;
		y = scale.y;
	}

	void Transform::setOrigin(float x, float y) {
		impl_->transformable.setOrigin(x, y);
	}

	void Transform::setOrigin(const float origin[2]) {
		impl_->transformable.setOrigin(origin[0], origin[1]);
	}

	void Transform::getOrigin(float& x, float& y) const {
		const auto& origin = impl_->transformable.getOrigin();
		x = origin.x;
		y = origin.y;
	}

	void* Transform::getInternalTransform() {
		return &impl_->transformable;
	}

	const void* Transform::getInternalTransform() const {
		return &impl_->transformable;
	}
}

