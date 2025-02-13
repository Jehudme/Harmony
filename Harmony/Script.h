#pragma once
#include "Object.h"

#define HARMONY_REGISTER_SCRIPT(NAME)							\
struct NAME;													\
namespace { auto _create_script_##NAME = []() -> bool {			\
        Harmony::Script::create<NAME>(#NAME); return true; }(); \
}																\

namespace Harmony
{
	class SceneNode;
	class TaskQueue;
	struct Script : public Object
	{
	public:
		Script();

		template<typename Type>
		static void create(const std::string& name);


	public:
		virtual void onEnter(std::shared_ptr<Object> object);
		virtual void onExit(std::shared_ptr<Object> object);

		virtual void onUpdate(std::shared_ptr<Object> object, const sf::Time& time, TaskQueue& taskQueue);
		virtual void onDraw(std::shared_ptr<const Object> object, sf::RenderTarget& renderTarget, sf::RenderStates states);
	};

	template<typename Type>
	inline void Script::create(const std::string& name)
	{
		auto script = Harmony::create<Type>();
		script->setName(name);
		script->retainSelf();
	}
}

