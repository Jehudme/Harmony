#include "Engine.h"
#include "Harmony/Properties.h"
#include "Harmony/Logger.h"

int main() {
	Harmony::Logger::Initialize("harmony.log");

	Harmony::Engine engine;
	engine.Initialize(Harmony::Properties{});
	engine.Run();
	return 0;
}