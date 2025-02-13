#pragma once
#include "Object.h"

#define HARMONY_REGISTER_SCRIPT(NAME) \
struct NAME;\
namespace { \
auto _create_script_##NAME = []() -> bool \
    { \
        Harmony::Script::create<NAME>(#NAME); return true; \
    }(); \
} \

namespace Harmony
{
	struct Script : public Object
	{
	public:
		Script();

		template<typename Type>
		static void create(const std::string& name);


	public:
		virtual void onCreate();
		virtual void onDelete();

		virtual void onEnter();
		virtual void onExit();

		virtual void onUpdate();
		virtual void onDraw();
	};

	template<typename Type>
	inline void Script::create(const std::string& name)
	{
		auto script = Harmony::create<Type>();
		script->setName(name);
		script->retainSelf();
	}
}

