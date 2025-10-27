#pragma once

#include <memory>
#include <vector>
#include <string>

namespace Harmony::Components
{
	/// @brief Sprite component - efficient texture rendering with animation support
	/// Uses composition to hide SFML implementation details
	class Sprite
	{
	public:
		Sprite(const Utilities::Configuration& configuration, Scenes::Scene& scene);
		~Sprite();

		// Texture management
		void setTexture(Utilities::UUID textureId);
		Utilities::UUID getTextureId() const;

		// Texture rectangle (for sprite sheets)
		void setTextureRect(int left, int top, int width, int height);
		void getTextureRect(int& left, int& top, int& width, int& height) const;

		// Color/tint
		void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const;

		// Animation
		void setAnimation(const std::vector<std::pair<int, int>>& frames, float frameTime);
		void playAnimation(bool loop = true);
		void stopAnimation();
		void pauseAnimation();
		void resumeAnimation();
		bool isAnimationPlaying() const;
		void updateAnimation(float deltaTime);
		void setAnimationFrame(size_t frameIndex);
		size_t getCurrentFrame() const;

		// Flip
		void setFlipHorizontal(bool flip);
		void setFlipVertical(bool flip);
		bool isFlippedHorizontal() const;
		bool isFlippedVertical() const;

		// Internal access for rendering
		void* getInternalSprite();
		const void* getInternalSprite() const;

	private:
		struct SpriteImpl;
		std::unique_ptr<SpriteImpl> impl_;
	};
}
