#include "pch.h"

#include "Engine.h"
#include "Configuration.h"
#include "Logger.h"

int main() {
	try {
		// Initialize the logger
		Harmony::Utilities::Logger::initialize("Harmony.log");


		Harmony::Utilities::Configuration configuration;
		configuration.load("physics_configuration.json");

		Harmony::Engine engine(configuration);

		engine.start();
	}
	catch (const std::exception& e) {
		return EXIT_FAILURE;
	}
}