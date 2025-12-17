#include <Harmony/Logger.h>
#include <Harmony/Engine.h>

int main() {
    Harmony::Logger::initialize();

	Harmony::Properties properties;
	Harmony::Engine(properties).start();


    HARMONY_INFO("Hello world");
}