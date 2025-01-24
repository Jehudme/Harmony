#include "pch.h"

#include "Engine.h"
#include "Configuration.h"

int main() {
	harmony::configuration->load();
	auto engine = harmony::utilities::create<harmony::core::Engine>(harmony::configuration);
	engine->run();
}
