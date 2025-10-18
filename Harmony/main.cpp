#include "pch.h"

#include "Engine.h"
#include "Configuration.h"

int main() {
	Harmony::Utilities::Configuration configuration;
	configuration.set<std::string>({ "window", "title" }, "Harmony Engine 2");

	Harmony::Engine engine(configuration);

	engine.start();
}