#include "pch.h"

#include "Engine.h"
#include "Configuration.h"

int main() {
	Harmony::Utilities::Configuration configuration;
	configuration.load("configuration.json");

	Harmony::Engine engine(configuration);

	engine.start();
}