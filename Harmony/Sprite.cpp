#include "pch.h"
#include "Sprite.h"
#include "Configuration.h"
#include "ComponentManagement.h"
#include "Scene.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Logger.h"
#include <SFML/Graphics.hpp>

HARMONY_REGISTER_COMPONENT_WITH_BASE(sf::Drawable, Harmony::Components::Sprite, Sprite)

namespace Harmony::Components
{
	// Animation data
	struct AnimationData {
		std::vector<sf::IntRect> frames;
		float frameTime = 0.1f;
		float currentTime = 0.0f;
		size_t currentFrame = 0;
		bool playing = false;
		bool paused = false;
		bool loop = true;
	};

	// PImpl to hide sf::Sprite
	struct Sprite::SpriteImpl {
		sf::Sprite sprite;
		Utilities::UUID textureId = 0;
		AnimationData animation;
		bool flipHorizontal = false;
		bool flipVertical = false;
	};

	Sprite::Sprite(const Utilities::Configuration& configuration, Scenes::Scene& scene)
		: impl_(std::make_unique<SpriteImpl>())
	{
		try {
			// Load texture if specified
			if (auto textureId = configuration.get<Utilities::UUID>({ "texture" })) {
				impl_->textureId = textureId.value();
				try {
					auto& resource = scene.engine.resourceManager->get("texture", impl_->textureId);
					auto& texture = convert<Harmony::Resources::Texture>(resource);
					impl_->sprite.setTexture(texture.get());
					HARMONY_DEBUG("Sprite component loaded texture {}", impl_->textureId);
				}
				catch (const std::exception& e) {
					HARMONY_ERROR("Failed to load texture {} for Sprite: {}", impl_->textureId, e.what());
				}
			}
			else {
				HARMONY_WARN("Sprite component created without texture");
			}

			// Set texture rectangle if specified
			if (auto left = configuration.get<int>({ "texture_rect", "left" })) {
				int top = configuration.get<int>({ "texture_rect", "top" }).value_or(0);
				int width = configuration.get<int>({ "texture_rect", "width" }).value_or(0);
				int height = configuration.get<int>({ "texture_rect", "height" }).value_or(0);
				impl_->sprite.setTextureRect(sf::IntRect(left.value(), top, width, height));
			}

			// Set color/tint if specified
			if (auto r = configuration.get<int>({ "color", "r" })) {
				int g = configuration.get<int>({ "color", "g" }).value_or(255);
				int b = configuration.get<int>({ "color", "b" }).value_or(255);
				int a = configuration.get<int>({ "color", "a" }).value_or(255);
				impl_->sprite.setColor(sf::Color(r.value(), g, b, a));
			}

			// Set flip if specified
			impl_->flipHorizontal = configuration.get<bool>({ "flip_horizontal" }).value_or(false);
			impl_->flipVertical = configuration.get<bool>({ "flip_vertical" }).value_or(false);
			if (impl_->flipHorizontal || impl_->flipVertical) {
				sf::Vector2f scale(impl_->flipHorizontal ? -1.0f : 1.0f,
					impl_->flipVertical ? -1.0f : 1.0f);
				impl_->sprite.setScale(scale);
			}
		}
		catch (const std::exception& e) {
			HARMONY_ERROR("Failed to initialize Sprite component: {}", e.what());
			throw;
		}
	}

	Sprite::~Sprite() = default;

	void Sprite::setTexture(Utilities::UUID textureId) {
		impl_->textureId = textureId;
		HARMONY_DEBUG("Sprite texture set to {}", textureId);
	}

	Utilities::UUID Sprite::getTextureId() const {
		return impl_->textureId;
	}

	void Sprite::setTextureRect(int left, int top, int width, int height) {
		impl_->sprite.setTextureRect(sf::IntRect(left, top, width, height));
	}

	void Sprite::getTextureRect(int& left, int& top, int& width, int& height) const {
		const auto& rect = impl_->sprite.getTextureRect();
		left = rect.left;
		top = rect.top;
		width = rect.width;
		height = rect.height;
	}

	void Sprite::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		impl_->sprite.setColor(sf::Color(r, g, b, a));
	}

	void Sprite::getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const {
		const auto& color = impl_->sprite.getColor();
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}

	void Sprite::setAnimation(const std::vector<std::pair<int, int>>& frames, float frameTime) {
		impl_->animation.frames.clear();
		for (const auto& [width, height] : frames) {
			impl_->animation.frames.push_back(sf::IntRect(width, height, 0, 0));
		}
		impl_->animation.frameTime = frameTime;
		impl_->animation.currentFrame = 0;
		impl_->animation.currentTime = 0.0f;
	}

	void Sprite::playAnimation(bool loop) {
		impl_->animation.playing = true;
		impl_->animation.paused = false;
		impl_->animation.loop = loop;
		impl_->animation.currentFrame = 0;
		impl_->animation.currentTime = 0.0f;
	}

	void Sprite::stopAnimation() {
		impl_->animation.playing = false;
		impl_->animation.paused = false;
		impl_->animation.currentFrame = 0;
		impl_->animation.currentTime = 0.0f;
	}

	void Sprite::pauseAnimation() {
		impl_->animation.paused = true;
	}

	void Sprite::resumeAnimation() {
		impl_->animation.paused = false;
	}

	bool Sprite::isAnimationPlaying() const {
		return impl_->animation.playing && !impl_->animation.paused;
	}

	void Sprite::updateAnimation(float deltaTime) {
		if (!impl_->animation.playing || impl_->animation.paused || impl_->animation.frames.empty()) {
			return;
		}

		impl_->animation.currentTime += deltaTime;
		if (impl_->animation.currentTime >= impl_->animation.frameTime) {
			impl_->animation.currentTime = 0.0f;
			impl_->animation.currentFrame++;

			if (impl_->animation.currentFrame >= impl_->animation.frames.size()) {
				if (impl_->animation.loop) {
					impl_->animation.currentFrame = 0;
				}
				else {
					impl_->animation.currentFrame = impl_->animation.frames.size() - 1;
					impl_->animation.playing = false;
				}
			}

			impl_->sprite.setTextureRect(impl_->animation.frames[impl_->animation.currentFrame]);
		}
	}

	void Sprite::setAnimationFrame(size_t frameIndex) {
		if (frameIndex < impl_->animation.frames.size()) {
			impl_->animation.currentFrame = frameIndex;
			impl_->sprite.setTextureRect(impl_->animation.frames[frameIndex]);
		}
		else {
			HARMONY_WARN("Sprite animation frame index {} out of range", frameIndex);
		}
	}

	size_t Sprite::getCurrentFrame() const {
		return impl_->animation.currentFrame;
	}

	void Sprite::setFlipHorizontal(bool flip) {
		impl_->flipHorizontal = flip;
		sf::Vector2f scale(flip ? -1.0f : 1.0f,
			impl_->flipVertical ? -1.0f : 1.0f);
		impl_->sprite.setScale(scale);
	}

	void Sprite::setFlipVertical(bool flip) {
		impl_->flipVertical = flip;
		sf::Vector2f scale(impl_->flipHorizontal ? -1.0f : 1.0f,
			flip ? -1.0f : 1.0f);
		impl_->sprite.setScale(scale);
	}

	bool Sprite::isFlippedHorizontal() const {
		return impl_->flipHorizontal;
	}

	bool Sprite::isFlippedVertical() const {
		return impl_->flipVertical;
	}

	void* Sprite::getInternalSprite() {
		return &impl_->sprite;
	}

	const void* Sprite::getInternalSprite() const {
		return &impl_->sprite;
	}
}
