
#include <Harmony/Engine.h>
#include <Harmony/Logger.h>

int main()
{
	Harmony::Logger::initialize("HarmonyLog.txt");
	Harmony::Internals::Engine({}).start();
}