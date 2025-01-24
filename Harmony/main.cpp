#include "pch.h"

#include "Engine.h"
#include "Configuration.h"

int main() {
	harmony::configuration->load();
	auto engine = harmony::utilities::onEnter<harmony::core::Engine>(1);
	engine->initializeWindow(10000);
	engine->run();
}
