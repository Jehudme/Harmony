// Task Usage Examples for Harmony Engine
// This file contains practical examples of using the task system

#include "Harmony.h"
#include "Engine.h"
#include "SceneTask.h"
#include "StateTask.h"
#include "EngineTask.h"
#include "ResourceTask.h"
#include "ComponentTask.h"
#include "UtilityTask.h"

namespace Examples {

// Example 1: Creating entities asynchronously during gameplay
void SpawnEnemyWave(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId, int enemyCount)
{
    std::vector<Harmony::Utilities::Configuration> enemyConfigs;
    
    for (int i = 0; i < enemyCount; ++i) {
        // Each enemy gets a configuration
        auto config = engine.configuration.subsection({"enemies", "basic_enemy"}).value();
        enemyConfigs.push_back(config);
    }
    
    // Create all enemies in a batch operation
    auto task = std::make_unique<Harmony::Tasks::BatchCreateEntitiesTask>(sceneId, enemyConfigs);
    engine.taskManagement->submit(std::move(task));
}

// Example 2: Projectile with automatic destruction after timeout
void FireProjectile(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId)
{
    // Create projectile entity
    auto config = engine.configuration.subsection({"entities", "projectile"}).value();
    auto createTask = std::make_unique<Harmony::Tasks::CreateEntityTask>(sceneId, config);
    
    // Store the entity ID somehow (in practice, you'd need to get this from the task result)
    // For this example, we'll assume we have the ID
    Harmony::Scenes::EntityID projectileId = 12345; // Example ID
    
    engine.taskManagement->submit(std::move(createTask));
    
    // Schedule destruction after 5 seconds
    auto destroyTask = std::make_unique<Harmony::Tasks::DelayedActionTask>(
        [&engine, sceneId, projectileId]() {
            auto task = std::make_unique<Harmony::Tasks::DestroyEntityTask>(sceneId, projectileId);
            engine.taskManagement->submit(std::move(task));
        },
        std::chrono::milliseconds(5000)
    );
    
    engine.taskManagement->submit(std::move(destroyTask));
}

// Example 3: Level transition with loading screen
void TransitionToNextLevel(Harmony::Engine& engine, Harmony::Utilities::UUID nextSceneId, Harmony::Utilities::UUID loadingStateId)
{
    // Step 1: Show loading screen
    auto showLoadingTask = std::make_unique<Harmony::Tasks::PushState>(loadingStateId);
    engine.taskManagement->submit(std::move(showLoadingTask));
    
    // Step 2: Unload current level resources
    auto unloadTask = std::make_unique<Harmony::Tasks::UnloadAllResourcesTask>();
    engine.taskManagement->submit(std::move(unloadTask));
    
    // Step 3: Load next scene (this runs in background)
    auto loadSceneTask = std::make_unique<Harmony::Tasks::CreateSceneTask>(nextSceneId);
    engine.taskManagement->submit(std::move(loadSceneTask));
    
    // Step 4: After loading completes, hide loading screen
    auto hideLoadingTask = std::make_unique<Harmony::Tasks::DelayedActionTask>(
        [&engine]() {
            auto task = std::make_unique<Harmony::Tasks::PopState>();
            engine.taskManagement->submit(std::move(task));
        },
        std::chrono::milliseconds(2000) // Wait 2 seconds minimum
    );
    
    engine.taskManagement->submit(std::move(hideLoadingTask));
}

// Example 4: Dynamic difficulty adjustment
void AdjustDifficultyBasedOnPerformance(Harmony::Engine& engine)
{
    // Check FPS and adjust if needed
    auto checkTask = std::make_unique<Harmony::Tasks::ConditionalTask>(
        [](Harmony::Engine& engine) {
            // If FPS is low, we should reduce difficulty
            return engine.getTargetFPS() > 0 && engine.getDeltaTime() > 0.033f; // > 30 FPS issue
        },
        [](Harmony::Engine& engine) {
            // Reduce quality or enemy count
            HARMONY_INFO("Performance issue detected, reducing difficulty");
            // Additional logic here to reduce game complexity
        },
        nullptr, // No action if performance is fine
        75 // Medium-high priority
    );
    
    engine.taskManagement->submit(std::move(checkTask));
}

// Example 5: Batch resource preloading for next level
void PreloadNextLevelAssets(Harmony::Engine& engine, int levelNumber)
{
    std::vector<std::pair<std::string, Harmony::Utilities::UUID>> resources;
    
    // Load textures
    resources.push_back({"texture", 1000 + levelNumber});
    resources.push_back({"texture", 2000 + levelNumber});
    
    // Load sounds
    resources.push_back({"sound", 3000 + levelNumber});
    resources.push_back({"sound", 4000 + levelNumber});
    
    // Load music
    resources.push_back({"music", 5000 + levelNumber});
    
    // Load fonts (if level-specific)
    resources.push_back({"font", 6000 + levelNumber});
    
    auto preloadTask = std::make_unique<Harmony::Tasks::BatchLoadResourcesTask>(resources);
    engine.taskManagement->submit(std::move(preloadTask));
    
    HARMONY_INFO("Preloading assets for level {}", levelNumber);
}

// Example 6: Power-up system
void ApplyPowerUp(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId, Harmony::Scenes::EntityID playerId, const std::string& powerUpType)
{
    // Get power-up configuration
    auto config = engine.configuration.subsection({"powerups", powerUpType}).value();
    
    // Add power-up component to player
    auto addComponentTask = std::make_unique<Harmony::Tasks::AddComponentTask>(
        sceneId, 
        playerId, 
        powerUpType, 
        config
    );
    
    engine.taskManagement->submit(std::move(addComponentTask));
    
    // Schedule power-up removal after duration
    int duration = config.get<int>({"duration"}).value_or(10000);
    
    auto removeTask = std::make_unique<Harmony::Tasks::DelayedActionTask>(
        [&engine, sceneId, playerId, powerUpType]() {
            HARMONY_INFO("Power-up {} expired", powerUpType);
            // Logic to remove component would go here
        },
        std::chrono::milliseconds(duration)
    );
    
    engine.taskManagement->submit(std::move(removeTask));
}

// Example 7: Auto-save system
void SetupAutoSave(Harmony::Engine& engine)
{
    // Save game every 5 minutes
    auto autoSaveTask = std::make_unique<Harmony::Tasks::RepeatedTask>(
        [](Harmony::Engine& engine) {
            // Save game state
            HARMONY_INFO("Auto-saving game...");
            // Save logic here
            HARMONY_INFO("Game saved successfully");
        },
        12, // 12 iterations = 1 hour with 5 minute intervals
        std::chrono::milliseconds(300000) // 5 minutes
    );
    
    engine.taskManagement->submit(std::move(autoSaveTask));
}

// Example 8: Graceful shutdown sequence
void ShutdownGame(Harmony::Engine& engine)
{
    // Step 1: Show "Saving..." message
    auto logTask = std::make_unique<Harmony::Tasks::LogMessageTask>(
        "Saving game before exit...",
        Harmony::Tasks::LogMessageTask::Info
    );
    engine.taskManagement->submit(std::move(logTask));
    
    // Step 2: Save game (would need actual save logic)
    auto saveTask = std::make_unique<Harmony::Tasks::LambdaTask>(
        []() {
            // Save game logic
            HARMONY_INFO("Game saved");
        },
        100 // High priority
    );
    engine.taskManagement->submit(std::move(saveTask));
    
    // Step 3: Unload all resources
    auto unloadTask = std::make_unique<Harmony::Tasks::UnloadAllResourcesTask>();
    engine.taskManagement->submit(std::move(unloadTask));
    
    // Step 4: Clear state stack
    auto clearStatesTask = std::make_unique<Harmony::Tasks::ClearStatesTask>();
    engine.taskManagement->submit(std::move(clearStatesTask));
    
    // Step 5: Stop engine
    auto stopTask = std::make_unique<Harmony::Tasks::StopEngineTask>();
    engine.taskManagement->submit(std::move(stopTask));
}

// Example 9: Wave-based enemy spawning system
void StartWaveSystem(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId, int totalWaves)
{
    auto waveTask = std::make_unique<Harmony::Tasks::RepeatedTask>(
        [sceneId](Harmony::Engine& engine) {
            static int currentWave = 1;
            
            HARMONY_INFO("Starting wave {}", currentWave);
            
            // Spawn more enemies each wave
            int enemyCount = 5 + (currentWave * 2);
            
            std::vector<Harmony::Utilities::Configuration> enemyConfigs;
            for (int i = 0; i < enemyCount; ++i) {
                auto config = engine.configuration.subsection({"enemies", "basic_enemy"}).value();
                enemyConfigs.push_back(config);
            }
            
            auto spawnTask = std::make_unique<Harmony::Tasks::BatchCreateEntitiesTask>(
                sceneId, 
                enemyConfigs
            );
            engine.taskManagement->submit(std::move(spawnTask));
            
            currentWave++;
        },
        totalWaves,
        std::chrono::milliseconds(30000) // 30 seconds between waves
    );
    
    engine.taskManagement->submit(std::move(waveTask));
}

// Example 10: Dynamic menu state management
void NavigateToSettings(Harmony::Engine& engine, Harmony::Utilities::UUID settingsStateId)
{
    auto pushTask = std::make_unique<Harmony::Tasks::PushState>(settingsStateId);
    engine.taskManagement->submit(std::move(pushTask));
}

void ReturnFromSettings(Harmony::Engine& engine)
{
    auto popTask = std::make_unique<Harmony::Tasks::PopState>();
    engine.taskManagement->submit(std::move(popTask));
}

void ReturnToMainMenu(Harmony::Engine& engine, Harmony::Utilities::UUID mainMenuStateId)
{
    // Clear all states and go to main menu
    auto clearTask = std::make_unique<Harmony::Tasks::ClearStatesTask>();
    engine.taskManagement->submit(std::move(clearTask));
    
    auto pushTask = std::make_unique<Harmony::Tasks::PushState>(mainMenuStateId);
    engine.taskManagement->submit(std::move(pushTask));
}

} // namespace Examples

// Usage in your game:
/*
void YourGameCode(Harmony::Engine& engine) 
{
    // Spawn enemies
    Examples::SpawnEnemyWave(engine, currentSceneId, 10);
    
    // Setup auto-save
    Examples::SetupAutoSave(engine);
    
    // Preload next level
    Examples::PreloadNextLevelAssets(engine, 2);
    
    // Apply power-up
    Examples::ApplyPowerUp(engine, sceneId, playerId, "SpeedBoost");
    
    // Shutdown game
    Examples::ShutdownGame(engine);
}
*/
