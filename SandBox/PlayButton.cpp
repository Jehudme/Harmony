#include <Harmony/Script.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Harmony/Engine.h>
#include <Harmony/Rectangle.h>
#include <Harmony/Text.h>
#include <Harmony/StateStack.h>
#include <Harmony/Task.h>

namespace Pong
{
	HARMONY_REGISTER_SCRIPT(PlayButtonScript) : public Harmony::Script
	{
		inline void onEnter(Object * object)
		{
			clock.restart();
		}

		inline void onExit(Object * object)
		{
		}

		inline void onUpdate(std::shared_ptr<Object> object, const sf::Time & time, Harmony::TaskQueue & taskQueue)
		{
			auto button = std::static_pointer_cast<Harmony::Rectangle>(object);
			auto playText = Harmony::find<Harmony::Text>("PlayText");
			auto engine = Harmony::find<Harmony::Engine>("Engine");

			const auto mousePosition = sf::Mouse::getPosition(engine->getRenderWindow());

			static sf::Color defaultOutlineColor = { 0, 255, 255, 100 };
			static sf::Color onCursorOutlineColor = { 0, 255, 255, 255 };

			static sf::Color defaultPlayTextColor = { 200, 200, 255, 255 };
			static sf::Color onCursorPlayTextColor = { 0, 255, 255, 255 };

			if (button->getGlobalBound().contains(mousePosition.x, mousePosition.y))
			{
				button->sprite.setOutlineColor(onCursorOutlineColor);
				playText->sprite.setFillColor(onCursorPlayTextColor);

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					taskQueue.push(Harmony::create<Harmony::SwitchStateTask>("GameState", "Engine"));
				}
			}
			else
			{
				button->sprite.setOutlineColor(defaultOutlineColor);
				playText->sprite.setFillColor(defaultPlayTextColor);

			}
		}

		inline void onDraw(std::shared_ptr<const Object> object, sf::RenderTarget & renderTarget, sf::RenderStates states) const {

		}

		inline static sf::Clock clock;
	};
}

