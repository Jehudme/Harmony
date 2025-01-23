#include "pch.h"

#include "Engine.h"
#include "Configuration.h"

int main() {
	harmony::configuration->load();
	auto engine = harmony::utilities::create<harmony::core::Engine>();
	engine->initializeWindow(10000);
	engine->run();
}
