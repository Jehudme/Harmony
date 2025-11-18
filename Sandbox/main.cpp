#include "main.h"
#include "Harmony/Configuration.h"
#include "Harmony/Engine.h"

int main()
{
	Harmony::Configuration configuration;
	Harmony::Internals::Engine(configuration).start();
}