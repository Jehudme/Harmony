#include <Harmony/Logger.h>

int main()
{
	auto logger = Harmony::Core::Logger::create("TEST");

	logger->info("hello worlbhd {}" , 4);
}
