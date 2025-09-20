#include "pch.h"

#include "Engine.h"
#include "Configuration.h"

int main() {
	Harmony::Internals::Configuration configuration;
	Harmony::Internals::Engine engine(configuration);
}