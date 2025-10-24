#include "pch.h"
#include "Exceptions.h"
#include "RenderManager.h"
#include <stdexcept>
#include "Engine.h"
#include "Configuration.h"


namespace Harmony::Management 
{
    struct RenderManager::RenderManagerImpl {
		std::unique_ptr<sf::RenderTarget> renderTarget;
	};

    RenderManager::RenderManager(Engine& engine)
		: engine_(engine), type_(RenderTargetType::Window), renderTarget(std::make_unique<sf::RenderWindow>()) {

        HARMONY_INFO("Engine initializing...");

        // Load window settings from configuration
        std::optional<std::string>  title   = engine_.configuration.get<std::string>    ({ "window", "title" });
        std::optional<unsigned int> width   = engine_.configuration.get<unsigned int>   ({ "window", "width" });
        std::optional<unsigned int> height  = engine_.configuration.get<unsigned int>   ({ "window", "height" });
        std::optional<unsigned int> fps     = engine_.configuration.get<unsigned int>   ({ "window", "fps" });

        if (!title.has_value())     HARMONY_WARN("Window title not specified in configuration, using default: '{}'", title.value());
        if (!width.has_value())     HARMONY_WARN("Window width not specified in configuration, using default: {}", width.value());
        if (!height.has_value())    HARMONY_WARN("Window height not specified in configuration, using default: {}", height.value());

        if (width.value_or(60) == 0 || height.value_or(60) == 0)  throw Exceptions::InvalidRenderTargetException("Invalid window dimensions");


		this->create(sf::VideoMode(width.value_or(800), height.value_or(600)), title.value_or("Harmony - Unknow"));
        
        if (!isOpen()) {
            HARMONY_ERROR("Failed to create SFML window");
            throw Exceptions::EngineError("Window creation failed");
        }

        setFramerateLimit(fps.value_or(60));
        HARMONY_INFO("Window created: '{}' ({}x{})", title.value(), width.value(), height.value());
    }

    RenderManager::~RenderManager() = default;

    // -------------------------
    // Common (RenderTarget)
    // -------------------------
    void RenderManager::clear(const sf::Color& color) {
        renderTarget->clear(color);
    }

    void RenderManager::setView(const sf::View& view) {
        renderTarget->setView(view);
    }

    const sf::View& RenderManager::getView() const {
        return renderTarget->getView();
    }

    const sf::View& RenderManager::getDefaultView() const {
        return renderTarget->getDefaultView();
    }

    sf::IntRect RenderManager::getViewport(const sf::View& view) const {
        return renderTarget->getViewport(view);
    }

    sf::Vector2f RenderManager::mapPixelToCoords(sf::Vector2i point) const {
        return renderTarget->mapPixelToCoords(point);
    }

    sf::Vector2i RenderManager::mapCoordsToPixel(sf::Vector2f point) const {
        return renderTarget->mapCoordsToPixel(point);
    }

    sf::Vector2u RenderManager::getSize() const {
        return renderTarget->getSize();
    }

    void RenderManager::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
        renderTarget->draw(drawable, states);
    }

    void RenderManager::draw(const sf::Vertex* vertices, std::size_t count,
        sf::PrimitiveType type, const sf::RenderStates& states) {
        renderTarget->draw(vertices, count, type, states);
    }

    // -------------------------
    // Window-only
    // -------------------------
    bool RenderManager::isOpen() const {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->isOpen();
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::close() {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->close();
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    bool RenderManager::pollEvent(sf::Event& event) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->pollEvent(event);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    bool RenderManager::waitEvent(sf::Event& event) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->waitEvent(event);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::create(const sf::VideoMode& mode, const sf::String& title, sf::Uint32 style, const sf::ContextSettings& settings) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get())) {
            win->create(mode, title, style, settings);
            return;
        }
		throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setTitle(const sf::String& title) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setTitle(title);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setIcon(unsigned int width, unsigned int height, const sf::Uint8* pixels) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setIcon(width, height, pixels);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setVisible(bool visible) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setVisible(visible);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setVerticalSyncEnabled(bool enabled) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setVerticalSyncEnabled(enabled);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setFramerateLimit(unsigned int limit) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setFramerateLimit(limit);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setKeyRepeatEnabled(bool enabled) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setKeyRepeatEnabled(enabled);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setJoystickThreshold(float threshold) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setJoystickThreshold(threshold);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setMouseCursorVisible(bool visible) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setMouseCursorVisible(visible);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setMouseCursorGrabbed(bool grabbed) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setMouseCursorGrabbed(grabbed);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    void RenderManager::setMouseCursor(const sf::Cursor& cursor) {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->setMouseCursor(cursor);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }

    sf::WindowHandle RenderManager::getSystemHandle() const {
        if (auto* win = dynamic_cast<sf::RenderWindow*>(renderTarget.get()))
            return win->getSystemHandle();
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderWindow");
    }


    // -------------------------
    // Texture-only
    // -------------------------

    void RenderManager::display() {
        if (type_ == RenderTargetType::Window)
            return dynamic_cast<sf::RenderWindow*>(renderTarget.get())->display();

		if (type_ == RenderTargetType::Texture)
            return dynamic_cast<sf::RenderTexture*>(renderTarget.get())->display();

		throw Exceptions::InvalidRenderTargetException("RenderTarget is neither a RenderWindow nor a RenderTexture");
    }

    const sf::Texture& RenderManager::getTexture() const {
        if (auto* tex = dynamic_cast<sf::RenderTexture*>(renderTarget.get()))
            return tex->getTexture();
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderTexture");
    }

    void RenderManager::setSmooth(bool smooth) {
        if (auto* tex = dynamic_cast<sf::RenderTexture*>(renderTarget.get()))
            return tex->setSmooth(smooth);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderTexture");
    }

    bool RenderManager::isSmooth() const {
        if (auto* tex = dynamic_cast<sf::RenderTexture*>(renderTarget.get()))
            return tex->isSmooth();
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderTexture");
    }

    bool RenderManager::generateMipmap() {
        if (auto* tex = dynamic_cast<sf::RenderTexture*>(renderTarget.get()))
            return tex->generateMipmap();
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderTexture");
    }

    void RenderManager::setRepeated(bool repeated) {
        if (auto* tex = dynamic_cast<sf::RenderTexture*>(renderTarget.get()))
            return tex->setRepeated(repeated);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderTexture");
    }

    bool RenderManager::isRepeated() const {
        if (auto* tex = dynamic_cast<sf::RenderTexture*>(renderTarget.get()))
            return tex->isRepeated();
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderTexture");
    }

    bool RenderManager::setActive(bool active) {
        if (auto* tex = dynamic_cast<sf::RenderTexture*>(renderTarget.get()))
            return tex->setActive(active);
        throw Exceptions::InvalidRenderTargetException("RenderTarget is not a RenderTexture");
    }

    // -------------------------
    // Type
    // -------------------------
    RenderManager::RenderTargetType RenderManager::getType() const {
        return type_;
    }
    sf::RenderTarget& RenderManager::getRenderTarget() {
		return *renderTarget;
    }
}
