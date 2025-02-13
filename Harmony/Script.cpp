#include "pch.h"
#include "Script.h"

Harmony::Script::Script()
	: Object(NULL)
{
}

void Harmony::Script::onEnter(Object* object)
{
}

void Harmony::Script::onExit(Object* object)
{
}

void Harmony::Script::onEnter(std::shared_ptr<Object> object)
{
}

void Harmony::Script::onExit(std::shared_ptr<Object> object)
{
}

void Harmony::Script::onUpdate(std::shared_ptr<Object> object, const sf::Time& time, TaskQueue& taskQueue)
{
}

void Harmony::Script::onDraw(std::shared_ptr<const Object> object, sf::RenderTarget& renderTarget, sf::RenderStates states)
{
}
