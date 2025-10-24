#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace Harmony {
    class Engine;
}

namespace Harmony::Management
{
    class RenderManager
    {
    public:
        RenderManager(Engine& engine);
		~RenderManager();

        void clear(const sf::Color& color = sf::Color::Black);
        void setView(const sf::View& view);
        const sf::View& getView() const;
        const sf::View& getDefaultView() const;
        sf::IntRect getViewport(const sf::View& view) const;
        sf::Vector2f mapPixelToCoords(sf::Vector2i point) const;
        sf::Vector2i mapCoordsToPixel(sf::Vector2f point) const;
        sf::Vector2u getSize() const;

        void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
        void draw(const sf::Vertex* vertices, std::size_t count, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

        bool isOpen() const;
        void close();
        bool pollEvent(sf::Event& event);
        bool waitEvent(sf::Event& event);
		void create(const sf::VideoMode& mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

        void setTitle(const sf::String& title);
        void setIcon(unsigned int width, unsigned int height, const sf::Uint8* pixels);
        void setVisible(bool visible);
        void setVerticalSyncEnabled(bool enabled);
        void setFramerateLimit(unsigned int limit);
        void setKeyRepeatEnabled(bool enabled);
        void setJoystickThreshold(float threshold);
        void setMouseCursorVisible(bool visible);
        void setMouseCursorGrabbed(bool grabbed);
        void setMouseCursor(const sf::Cursor& cursor);

        sf::WindowHandle getSystemHandle() const;

        void display();
        const sf::Texture& getTexture() const;
        void setSmooth(bool smooth);
        bool isSmooth() const;
        bool generateMipmap();
        void setRepeated(bool repeated);
        bool isRepeated() const;
        bool setActive(bool active = true);

        enum class RenderTargetType
        {
            Window,
            Texture
		};

		RenderTargetType getType() const;

		sf::RenderTarget& getRenderTarget();

    private:
        struct RenderManagerImpl;

		std::unique_ptr<sf::RenderTarget> renderTarget;
		RenderTargetType type_;
        Engine& engine_;
    };
}