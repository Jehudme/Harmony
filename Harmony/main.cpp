#include "pch.h"

#include "Engine.h"
#include "Configuration.h"

int main() {
	Harmony::Internals::Configuration configuration;
	configuration.set<std::string>({ "window", "title" }, "Harmony Engine 2");

	Harmony::Internals::Engine engine(configuration);

	engine.start();
}