
#include <Harmony/Engine.h>
#include <Harmony/Logger.h>

using namespace Harmony;
using namespace Harmony::Internals;

int main()
{
	Harmony::Logger::initialize();
	Engine(Configuration()).start();
}