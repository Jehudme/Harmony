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

// Example 10: Pause/Resume game by disabling scene updates
void PauseGame(Harmony::Engine& engine, Harmony::Utilities::UUID gameSceneId)
{
    // Disable scene updating to pause game logic while keeping rendering
    auto task = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(task));
    HARMONY_INFO("Game paused - scene updates disabled");
}

void ResumeGame(Harmony::Engine& engine, Harmony::Utilities::UUID gameSceneId)
{
    // Re-enable scene updating to resume game logic
    auto task = std::make_unique<Harmony::Tasks::EnableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(task));
    HARMONY_INFO("Game resumed - scene updates enabled");
}

// Example 11: Hide/Show UI or background scenes
void HideBackgroundScene(Harmony::Engine& engine, Harmony::Utilities::UUID backgroundSceneId)
{
    // Disable drawing for a background scene to save performance
    auto task = std::make_unique<Harmony::Tasks::DisableSceneDrawingTask>(backgroundSceneId);
    engine.taskManagement->submit(std::move(task));
}

void ShowBackgroundScene(Harmony::Engine& engine, Harmony::Utilities::UUID backgroundSceneId)
{
    // Re-enable drawing for a background scene
    auto task = std::make_unique<Harmony::Tasks::EnableSceneDrawingTask>(backgroundSceneId);
    engine.taskManagement->submit(std::move(task));
}

// Example 12: Reset scene to initial state (e.g., retry level)
void RetryLevel(Harmony::Engine& engine, Harmony::Utilities::UUID levelSceneId)
{
    // Reset the scene to its initial configuration from JSON
    auto task = std::make_unique<Harmony::Tasks::ResetSceneTask>(levelSceneId);
    engine.taskManagement->submit(std::move(task));
    HARMONY_INFO("Level reset to initial state");
}

// Example 13: Cutscene playback with scene control
void PlayCutscene(Harmony::Engine& engine, Harmony::Utilities::UUID gameSceneId, Harmony::Utilities::UUID cutsceneSceneId)
{
    // Disable game scene updates during cutscene
    auto disableGameTask = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(gameSceneId);
    engine.taskManagement->submit(std::move(disableGameTask));
    
    // Enable cutscene scene
    auto enableCutsceneDrawTask = std::make_unique<Harmony::Tasks::EnableSceneDrawingTask>(cutsceneSceneId);
    auto enableCutsceneUpdateTask = std::make_unique<Harmony::Tasks::EnableSceneUpdatingTask>(cutsceneSceneId);
    engine.taskManagement->submit(std::move(enableCutsceneDrawTask));
    engine.taskManagement->submit(std::move(enableCutsceneUpdateTask));
    
    // After cutscene duration, restore game scene
    auto restoreTask = std::make_unique<Harmony::Tasks::DelayedActionTask>(
        [&engine, gameSceneId, cutsceneSceneId]() {
            auto enableGameTask = std::make_unique<Harmony::Tasks::EnableSceneUpdatingTask>(gameSceneId);
            engine.taskManagement->submit(std::move(enableGameTask));
            
            auto disableCutsceneDrawTask = std::make_unique<Harmony::Tasks::DisableSceneDrawingTask>(cutsceneSceneId);
            auto disableCutsceneUpdateTask = std::make_unique<Harmony::Tasks::DisableSceneUpdatingTask>(cutsceneSceneId);
            engine.taskManagement->submit(std::move(disableCutsceneDrawTask));
            engine.taskManagement->submit(std::move(disableCutsceneUpdateTask));
        },
        std::chrono::milliseconds(5000) // 5 second cutscene
    );
    engine.taskManagement->submit(std::move(restoreTask));
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

// ============================================================================
// NEW TASK EXAMPLES - Component and General Operations
// ============================================================================

// Example 14: Remove component from entity
void RemovePowerUp(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId, Harmony::Scenes::EntityID entityId)
{
    // Remove expired power-up component
    auto task = std::make_unique<Harmony::Tasks::RemoveComponentTask>(
        sceneId, entityId, "PowerUp"
    );
    engine.taskManagement->submit(std::move(task));
    HARMONY_INFO("Power-up removed from entity");
}

// Example 15: Batch add components to multiple entities
void AddShieldsToAllies(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId, 
    const std::vector<Harmony::Scenes::EntityID>& allyEntities)
{
    auto shieldConfig = engine.configuration.subsection({"components", "shield"}).value();
    
    auto task = std::make_unique<Harmony::Tasks::BatchAddComponentsTask>(
        sceneId, allyEntities, "Shield", shieldConfig
    );
    engine.taskManagement->submit(std::move(task));
    HARMONY_INFO("Shields applied to {} allies", allyEntities.size());
}

// Example 16: Batch remove components from multiple entities
void RemoveStatusEffects(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId,
    const std::vector<Harmony::Scenes::EntityID>& entities)
{
    // Remove status effect component from all affected entities
    auto task = std::make_unique<Harmony::Tasks::BatchRemoveComponentsTask>(
        sceneId, entities, "StatusEffect"
    );
    engine.taskManagement->submit(std::move(task));
    HARMONY_INFO("Status effects cleared from {} entities", entities.size());
}

// Example 17: Count entities with callback
void DisplayEntityCount(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId)
{
    auto task = std::make_unique<Harmony::Tasks::CountEntitiesTask>(
        sceneId,
        [](size_t count) {
            HARMONY_INFO("Current entity count: {}", count);
            // Update UI with entity count
        }
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 18: Clear all entities from scene
void ClearLevel(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId)
{
    // Remove all entities from the scene for level reset
    auto task = std::make_unique<Harmony::Tasks::ClearSceneEntitiesTask>(sceneId);
    engine.taskManagement->submit(std::move(task));
    HARMONY_INFO("All entities cleared from scene");
}

// Example 19: Sequential task execution
void LevelTransitionSequence(Harmony::Engine& engine, Harmony::Utilities::UUID oldSceneId, 
    Harmony::Utilities::UUID newSceneId)
{
    std::vector<std::function<void(Harmony::Engine&)>> sequence = {
        [oldSceneId](Harmony::Engine& e) {
            HARMONY_INFO("Step 1: Fading out current level");
        },
        [oldSceneId](Harmony::Engine& e) {
            HARMONY_INFO("Step 2: Saving progress");
        },
        [newSceneId](Harmony::Engine& e) {
            HARMONY_INFO("Step 3: Loading new level");
        },
        [newSceneId](Harmony::Engine& e) {
            HARMONY_INFO("Step 4: Fading in new level");
        }
    };
    
    auto task = std::make_unique<Harmony::Tasks::SequentialTasksTask>(sequence);
    engine.taskManagement->submit(std::move(task));
}

// Example 20: Timed action for performance measurement
void MeasurePhysicsPerformance(Harmony::Engine& engine)
{
    auto task = std::make_unique<Harmony::Tasks::TimedActionTask>(
        [](Harmony::Engine& e) {
            // Perform physics calculations
            HARMONY_DEBUG("Physics step executed");
        },
        "Physics Step"
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 21: Retry failed operations
void RetryServerConnection(Harmony::Engine& engine)
{
    auto task = std::make_unique<Harmony::Tasks::RetryTask>(
        [](Harmony::Engine& e) -> bool {
            // Attempt to connect to server
            bool success = false; // Simulate connection attempt
            if (success) {
                HARMONY_INFO("Server connection established");
                return true;
            }
            HARMONY_WARN("Server connection failed, will retry");
            return false;
        },
        5,  // Max 5 retries
        std::chrono::milliseconds(2000)  // 2 seconds between retries
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 22: Throttled save system
void ThrottledSave(Harmony::Engine& engine)
{
    // Save will execute at most once per 5 seconds
    auto task = std::make_unique<Harmony::Tasks::ThrottledTask>(
        [](Harmony::Engine& e) {
            HARMONY_INFO("Game saved");
            // Perform save operations
        },
        std::chrono::milliseconds(5000)  // Minimum 5 seconds between saves
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 23: Debounced input handling
void HandleSearchInput(Harmony::Engine& engine, const std::string& searchText)
{
    // Wait for user to stop typing before performing search
    auto task = std::make_unique<Harmony::Tasks::DebouncedTask>(
        [searchText](Harmony::Engine& e) {
            HARMONY_INFO("Searching for: {}", searchText);
            // Perform search operation
        },
        std::chrono::milliseconds(500)  // Wait 500ms after last input
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 24: Memory snapshot for debugging
void CheckMemoryUsage(Harmony::Engine& engine)
{
    auto task = std::make_unique<Harmony::Tasks::MemorySnapshotTask>(
        [](std::string snapshot) {
            HARMONY_DEBUG("Memory snapshot: {}", snapshot);
        }
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 25: Dump scene information
void DebugScene(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId)
{
    auto task = std::make_unique<Harmony::Tasks::DumpSceneInfoTask>(sceneId);
    engine.taskManagement->submit(std::move(task));
}

// Example 26: Validate scene integrity
void ValidateLevel(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId)
{
    auto task = std::make_unique<Harmony::Tasks::ValidateSceneTask>(
        sceneId,
        [](bool isValid, std::string message) {
            if (isValid) {
                HARMONY_INFO("Scene validation passed: {}", message);
            } else {
                HARMONY_ERROR("Scene validation failed: {}", message);
            }
        }
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 27: Chained tasks with error handling
void ComplexInitialization(Harmony::Engine& engine)
{
    std::vector<std::function<bool(Harmony::Engine&)>> tasks = {
        [](Harmony::Engine& e) -> bool {
            HARMONY_INFO("Initializing graphics");
            return true;  // Success
        },
        [](Harmony::Engine& e) -> bool {
            HARMONY_INFO("Initializing audio");
            return true;  // Success
        },
        [](Harmony::Engine& e) -> bool {
            HARMONY_INFO("Loading configuration");
            return true;  // Success
        }
    };
    
    auto task = std::make_unique<Harmony::Tasks::ChainedTasksTask>(
        tasks,
        [](Harmony::Engine& e, int failedIndex) {
            HARMONY_ERROR("Initialization failed at step {}", failedIndex);
        }
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 28: Scheduled event
void ScheduleDailyBonus(Harmony::Engine& engine)
{
    // Schedule bonus to trigger in 24 hours
    auto targetTime = std::chrono::steady_clock::now() + std::chrono::hours(24);
    
    auto task = std::make_unique<Harmony::Tasks::ScheduledTask>(
        [](Harmony::Engine& e) {
            HARMONY_INFO("Daily bonus awarded!");
            // Award bonus to player
        },
        targetTime
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 29: Interval timer (countdown)
void StartGameTimer(Harmony::Engine& engine, int seconds)
{
    auto task = std::make_unique<Harmony::Tasks::IntervalTask>(
        [remaining = seconds](Harmony::Engine& e) mutable -> bool {
            HARMONY_INFO("Time remaining: {} seconds", remaining);
            remaining--;
            return remaining > 0;  // Continue while time remains
        },
        std::chrono::milliseconds(1000),  // 1 second interval
        seconds  // Max executions
    );
    engine.taskManagement->submit(std::move(task));
}

// Example 30: Check if scene exists before operations
void SafeSceneOperation(Harmony::Engine& engine, Harmony::Utilities::UUID sceneId)
{
    auto checkTask = std::make_unique<Harmony::Tasks::CheckSceneExistsTask>(
        sceneId,
        [sceneId](bool exists) {
            if (exists) {
                HARMONY_INFO("Scene exists, proceeding with operation");
                // Perform scene operation (would need engine reference passed differently)
            } else {
                HARMONY_WARN("Scene does not exist, skipping operation");
            }
        }
    );
    engine.taskManagement->submit(std::move(checkTask));
}

// Example 31: Batch move entities between scenes
void MoveUnitsToNewLocation(Harmony::Engine& engine, 
    Harmony::Utilities::UUID sourceSceneId,
    Harmony::Utilities::UUID targetSceneId,
    const std::vector<Harmony::Scenes::EntityID>& units)
{
    auto task = std::make_unique<Harmony::Tasks::BatchMoveEntitiesTask>(
        sourceSceneId, targetSceneId, units
    );
    engine.taskManagement->submit(std::move(task));
    HARMONY_INFO("Moving {} units to new scene", units.size());
}

} // namespace Examples
