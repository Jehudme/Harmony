#include "pch.h"

#include "Engine.h"
#include "Configuration.h"
#include "Script.h"
#include <iostream>
extern void Script1(CREATE_SCRIPT_ARGUMENTS) {
}
REGISTER_CREATE_SCRIPT(Script1);

int main() {
	harmony::configuration->load();
	auto engine = harmony::utilities::create<harmony::core::Engine>(harmony::configuration);
	engine->run();
}
