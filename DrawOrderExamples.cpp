// Draw Order Examples for Harmony Engine
// This file demonstrates the new scene draw order functionality

#include "Harmony.h"
#include "Engine.h"
#include "SceneTask.h"
#include "StateTask.h"

namespace DrawOrderExamples {

// Example 1: Creating scenes with different draw orders
// Background (drawOrder: 0), Game (drawOrder: 10), UI (drawOrder: 20)
void SetupLayeredScenes(Harmony::Engine& engine)
{
    // Scene 1001: Background layer (drawOrder: 0 in config)
    // Will be drawn first (behind everything)
    auto createBgTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(1001);
    engine.taskManagement->submit(std::move(createBgTask));
    
    // Scene 1002: Game layer (drawOrder: 10 in config)
    // Will be drawn second (middle layer)
    auto createGameTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(1002);
    engine.taskManagement->submit(std::move(createGameTask));
    
    // Scene 1003: UI layer (drawOrder: 20 in config)
    // Will be drawn last (on top of everything)
    auto createUITask = std::make_unique<Harmony::Tasks::CreateSceneTask>(1003);
    engine.taskManagement->submit(std::move(createUITask));
    
    HARMONY_INFO("Layered scenes created with proper draw order");
}

// Example 2: Dynamically adding a new scene to the current state
void AddNewSceneDynamically(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId)
{
    // The CreateSceneTask will automatically add the scene to the current state
    // The scene will be drawn according to its drawOrder property
    auto createTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(sceneId);
    engine.taskManagement->submit(std::move(createTask));
    
    HARMONY_INFO("Scene {} will be added to current state with its configured drawOrder", sceneId);
}

// Example 3: Removing a scene from the current state
void RemoveSceneFromState(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId)
{
    // The DeleteSceneTask removes the scene from the current state
    // The scene will no longer be drawn or updated
    auto deleteTask = std::make_unique<Harmony::Tasks::DeleteSceneTask>(sceneId);
    engine.taskManagement->submit(std::move(deleteTask));
    
    HARMONY_INFO("Scene {} will be removed from current state", sceneId);
}

// Example 4: Temporarily hide a layer (e.g., hide UI during cutscene)
void HideUILayer(Harmony::Engine& engine, Harmony::Utilities::UUID uiSceneId)
{
    // Disable drawing for the UI scene
    auto hideTask = std::make_unique<Harmony::Tasks::DisableSceneDrawingTask>(uiSceneId);
    engine.taskManagement->submit(std::move(hideTask));
    
    HARMONY_INFO("UI layer hidden");
}

void ShowUILayer(Harmony::Engine& engine, Harmony::Utilities::UUID uiSceneId)
{
    // Re-enable drawing for the UI scene
    auto showTask = std::make_unique<Harmony::Tasks::EnableSceneDrawingTask>(uiSceneId);
    engine.taskManagement->submit(std::move(showTask));
    
    HARMONY_INFO("UI layer shown");
}

// Example 5: Multi-layer game setup with proper ordering
void SetupMultiLayerGame(Harmony::Engine& engine)
{
    // Layer 1: Parallax Background (drawOrder: -10)
    auto bgTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(100);
    engine.taskManagement->submit(std::move(bgTask));
    
    // Layer 2: Game World (drawOrder: 0)
    auto worldTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(200);
    engine.taskManagement->submit(std::move(worldTask));
    
    // Layer 3: Particles/Effects (drawOrder: 5)
    auto effectsTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(300);
    engine.taskManagement->submit(std::move(effectsTask));
    
    // Layer 4: UI Elements (drawOrder: 10)
    auto uiTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(400);
    engine.taskManagement->submit(std::move(uiTask));
    
    // Layer 5: Debug Overlay (drawOrder: 100)
    auto debugTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(500);
    engine.taskManagement->submit(std::move(debugTask));
    
    HARMONY_INFO("Multi-layer game setup complete");
}

// Example 6: Dynamic scene management - add/remove scenes during gameplay
void ManageDynamicScenes(Harmony::Engine& engine)
{
    // Add a boss arena scene when entering boss fight
    Harmony::Utilities::UUID bossArenaSceneId = 9001;
    auto addBossScene = std::make_unique<Harmony::Tasks::CreateSceneTask>(bossArenaSceneId);
    engine.taskManagement->submit(std::move(addBossScene));
    
    // Later, when boss is defeated, remove the arena scene
    auto removeBossScene = std::make_unique<Harmony::Tasks::DeleteSceneTask>(bossArenaSceneId);
    engine.taskManagement->submit(std::move(removeBossScene));
}

// Example 7: Cutscene system using scene ordering
void PlayCutsceneWithLayers(Harmony::Engine& engine, 
                            Harmony::Utilities::UUID gameSceneId, 
                            Harmony::Utilities::UUID cutsceneSceneId,
                            Harmony::Utilities::UUID uiSceneId)
{
    // Step 1: Pause game scene (stop updates but keep visible)
    auto pauseGameTask = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(pauseGameTask));
    
    // Step 2: Hide UI during cutscene
    auto hideUITask = std::make_unique<Harmony::Tasks::DisableSceneDrawingTask>(uiSceneId);
    engine.taskManagement->submit(std::move(hideUITask));
    
    // Step 3: Create and show cutscene (with appropriate drawOrder)
    auto createCutsceneTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(cutsceneSceneId);
    engine.taskManagement->submit(std::move(createCutsceneTask));
    
    // Step 4: After cutscene duration, restore normal state
    auto restoreTask = std::make_unique<Harmony::Tasks::DelayedActionTask>(
        [&engine, gameSceneId, cutsceneSceneId, uiSceneId]() {
            // Remove cutscene
            auto deleteTask = std::make_unique<Harmony::Tasks::DeleteSceneTask>(cutsceneSceneId);
            engine.taskManagement->submit(std::move(deleteTask));
            
            // Resume game
            auto resumeTask = std::make_unique<Harmony::Tasks::EnableSceneUpdatingTask>(gameSceneId);
            engine.taskManagement->submit(std::move(resumeTask));
            
            // Show UI again
            auto showUITask = std::make_unique<Harmony::Tasks::EnableSceneDrawingTask>(uiSceneId);
            engine.taskManagement->submit(std::move(showUITask));
            
            HARMONY_INFO("Cutscene finished, game resumed");
        },
        std::chrono::milliseconds(5000) // 5 second cutscene
    );
    engine.taskManagement->submit(std::move(restoreTask));
}

// Example 8: Menu system with overlays
void ShowPauseMenu(Harmony::Engine& engine, 
                   Harmony::Utilities::UUID gameSceneId,
                   Harmony::Utilities::UUID pauseMenuSceneId)
{
    // Pause the game (stop updates)
    auto pauseTask = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(pauseTask));
    
    // Show pause menu overlay (should have high drawOrder to appear on top)
    auto showMenuTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(pauseMenuSceneId);
    engine.taskManagement->submit(std::move(showMenuTask));
    
    HARMONY_INFO("Pause menu shown - game paused");
}

void HidePauseMenu(Harmony::Engine& engine,
                   Harmony::Utilities::UUID gameSceneId,
                   Harmony::Utilities::UUID pauseMenuSceneId)
{
    // Remove pause menu
    auto hideMenuTask = std::make_unique<Harmony::Tasks::DeleteSceneTask>(pauseMenuSceneId);
    engine.taskManagement->submit(std::move(hideMenuTask));
    
    // Resume the game
    auto resumeTask = std::make_unique<Harmony::Tasks::EnableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(resumeTask));
    
    HARMONY_INFO("Pause menu hidden - game resumed");
}

// Example 9: Weather effects overlay
void AddWeatherEffect(Harmony::Engine& engine, const std::string& weatherType)
{
    // Weather effects should have a drawOrder between game and UI
    // For example: game (0), weather (8), UI (10)
    Harmony::Utilities::UUID weatherSceneId = 7000;
    
    if (weatherType == "rain") {
        weatherSceneId = 7001;
    } else if (weatherType == "snow") {
        weatherSceneId = 7002;
    } else if (weatherType == "fog") {
        weatherSceneId = 7003;
    }
    
    auto addWeatherTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(weatherSceneId);
    engine.taskManagement->submit(std::move(addWeatherTask));
    
    HARMONY_INFO("Weather effect '{}' added", weatherType);
}

void RemoveWeatherEffect(Harmony::Engine& engine, Harmony::Utilities::UUID weatherSceneId)
{
    auto removeTask = std::make_unique<Harmony::Tasks::DeleteSceneTask>(weatherSceneId);
    engine.taskManagement->submit(std::move(removeTask));
    
    HARMONY_INFO("Weather effect removed");
}

// Example 10: Debug overlay toggle
void ToggleDebugOverlay(Harmony::Engine& engine, 
                        Harmony::Utilities::UUID debugSceneId,
                        bool& debugEnabled)
{
    if (!debugEnabled) {
        // Create debug overlay with very high drawOrder (e.g., 999)
        auto createTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(debugSceneId);
        engine.taskManagement->submit(std::move(createTask));
        debugEnabled = true;
        HARMONY_INFO("Debug overlay enabled");
    } else {
        // Remove debug overlay
        auto deleteTask = std::make_unique<Harmony::Tasks::DeleteSceneTask>(debugSceneId);
        engine.taskManagement->submit(std::move(deleteTask));
        debugEnabled = false;
        HARMONY_INFO("Debug overlay disabled");
    }
}

} // namespace DrawOrderExamples

/*
Configuration example for scenes with drawOrder:

{
  "scenes": {
    "100": {
      "drawOrder": -10,
      "entities": [...]  // Background parallax
    },
    "200": {
      "drawOrder": 0,
      "entities": [...]  // Game world
    },
    "300": {
      "drawOrder": 5,
      "entities": [...]  // Particle effects
    },
    "400": {
      "drawOrder": 10,
      "entities": [...]  // UI elements
    },
    "500": {
      "drawOrder": 100,
      "entities": [...]  // Debug overlay
    },
    "7001": {
      "drawOrder": 8,
      "entities": [...]  // Rain effect
    },
    "9001": {
      "drawOrder": 2,
      "entities": [...]  // Boss arena
    }
  }
}

Usage in game:
void GameInit(Harmony::Engine& engine) {
    DrawOrderExamples::SetupMultiLayerGame(engine);
    DrawOrderExamples::AddWeatherEffect(engine, "rain");
}

void OnBossEncounter(Harmony::Engine& engine) {
    DrawOrderExamples::ManageDynamicScenes(engine);
}

void OnPauseKey(Harmony::Engine& engine) {
    DrawOrderExamples::ShowPauseMenu(engine, gameSceneId, pauseMenuSceneId);
}
*/
