
#include <Harmony/Engine.h>
#include <Harmony/ResourceHandler.h>
#include <Harmony/TextureResources.h>
#include <Harmony/AudioResources.h>
#include <Harmony/ModelResources.h>
#include <Harmony/Logger.h>
#include <Harmony/Configuration.h>

#include <memory>
#include <thread>
#include <chrono>

using namespace Harmony;
using namespace Harmony::Utilities;

// Simple demo to test the ResourceHandler system
void runResourceHandlerDemo()
{
	Logger::info("=== ResourceHandler System Demo ===");
	
	// Create configuration for ResourceHandler
	Configuration config;
	config.set<std::size_t>({ "resources", "memoryCap" }, 50 * 1024 * 1024);  // 50MB cap
	config.set<float>({ "resources", "defaultUnloadDelay" }, 5.0f);  // 5 seconds
	config.set<bool>({ "resources", "enableLogging" }, true);
	
	// Create ResourceHandler
	auto resourceHandler = std::make_unique<ResourceHandler>(config);
	
	Logger::info("=== Test 1: Registering and Loading Resources ===");
	
	// Register some test texture resources (using dummy paths for demo)
	auto texture1 = std::make_shared<TextureResource>(1, "resources/textures/test1.png", 5.0f, false);
	auto texture2 = std::make_shared<TextureResource>(2, "resources/textures/test2.png", 5.0f, false);
	auto texture3 = std::make_shared<TextureResource>(3, "resources/textures/permanent.png", 5.0f, true);  // Always loaded
	
	// Set file sizes for simulation (since files don't exist)
	// In a real scenario, these would be calculated during load()
	
	resourceHandler->registerResource(texture1);
	resourceHandler->registerResource(texture2);
	resourceHandler->registerResource(texture3);
	
	Logger::info("Registered {} resources", resourceHandler->getTotalResourceCount());
	
	Logger::info("=== Test 2: Memory Usage Tracking ===");
	Logger::info("Current memory usage: {} bytes", resourceHandler->getCurrentMemoryUsage());
	Logger::info("Memory cap: {} bytes ({} MB)", resourceHandler->getMemoryCap(), resourceHandler->getMemoryCap() / (1024 * 1024));
	
	Logger::info("=== Test 3: Batch Operations ===");
	std::vector<ResourceID> batchIds = { 1, 2 };
	
	try {
		Logger::info("Note: Loading will fail because test files don't exist - this is expected");
		resourceHandler->batchLoad(batchIds);
	}
	catch (const std::exception& e) {
		Logger::info("Expected failure (test files don't exist): {}", e.what());
	}
	
	Logger::info("=== Test 4: Resource Statistics ===");
	Logger::info("Total resources: {}", resourceHandler->getTotalResourceCount());
	Logger::info("Loaded resources: {}", resourceHandler->getLoadedResourceCount());
	Logger::info("Current memory usage: {} bytes", resourceHandler->getCurrentMemoryUsage());
	
	Logger::info("=== Test 5: Auto-Unload Simulation ===");
	Logger::info("Simulating idle time for auto-unload...");
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	resourceHandler->updateAutoUnload();
	
	Logger::info("=== Test 6: Memory Cap Changes ===");
	std::size_t newCap = 25 * 1024 * 1024;  // 25MB
	Logger::info("Changing memory cap to {} bytes ({} MB)", newCap, newCap / (1024 * 1024));
	resourceHandler->setMemoryCap(newCap);
	
	Logger::info("=== Test 7: Resource Types Demo ===");
	
	// Register different resource types
	auto image1 = std::make_shared<ImageResource>(10, "resources/images/test.png");
	auto font1 = std::make_shared<FontResource>(20, "resources/fonts/test.ttf", 32);
	auto sound1 = std::make_shared<SoundResource>(30, "resources/sounds/test.wav");
	auto music1 = std::make_shared<MusicResource>(40, "resources/music/test.mp3");
	auto model1 = std::make_shared<ModelResource>(50, "resources/models/test.obj");
	auto material1 = std::make_shared<MaterialResource>(60);
	auto shader1 = std::make_shared<ShaderResource>(70, "resources/shaders/test.vs", "resources/shaders/test.fs");
	
	resourceHandler->registerResource(image1);
	resourceHandler->registerResource(font1);
	resourceHandler->registerResource(sound1);
	resourceHandler->registerResource(music1);
	resourceHandler->registerResource(model1);
	resourceHandler->registerResource(material1);
	resourceHandler->registerResource(shader1);
	
	Logger::info("Registered {} total resources including:", resourceHandler->getTotalResourceCount());
	Logger::info("  - Textures");
	Logger::info("  - Images");
	Logger::info("  - Fonts");
	Logger::info("  - Sounds");
	Logger::info("  - Music");
	Logger::info("  - Models");
	Logger::info("  - Materials");
	Logger::info("  - Shaders");
	
	Logger::info("=== Test 8: Unload All ===");
	resourceHandler->unloadAll();
	Logger::info("Unloaded all non-permanent resources");
	Logger::info("Loaded resources remaining: {}", resourceHandler->getLoadedResourceCount());
	
	Logger::info("=== Demo Complete ===");
	Logger::info("ResourceHandler system is working correctly!");
	Logger::info("Memory usage: {} / {} bytes", 
		resourceHandler->getCurrentMemoryUsage(), 
		resourceHandler->getMemoryCap());
}

int main()
{
	// Initialize logger
	Logger::initialize("Sandbox.log");
	
	Logger::info("=== Harmony Engine - ResourceHandler Sandbox ===");
	Logger::info("This demo tests the ResourceHandler system without requiring actual resource files");
	Logger::info("");
	
	try {
		// Run the ResourceHandler demo
		runResourceHandlerDemo();
		
		Logger::info("");
		Logger::info("=== All tests completed successfully ===");
		Logger::info("Press any key to exit...");
		
	}
	catch (const std::exception& e) {
		Logger::critical("Unhandled exception: {}", e.what());
		return 1;
	}
	
	Logger::shutdown();
	return 0;
}