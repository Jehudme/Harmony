#include "pch.h"

#include "Engine.h"
#include "Configuration.h"
#include "Script.h"
#include <iostream>

REGISTER_CREATE_SCRIPT(Script1);

static inline void Script1(CREATE_SCRIPT_ARGUMENTS) {
}

int main() {
	harmony::configuration->load();
	auto engine = harmony::utilities::create<harmony::core::Engine>(harmony::configuration);
	engine->run();
}