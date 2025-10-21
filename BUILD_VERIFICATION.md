# Build Verification Checklist

This checklist should be used to verify the refactored code builds and works correctly on Windows.

## Prerequisites

- [ ] Windows 10 or later
- [ ] Visual Studio 2019 or later with C++ development tools
- [ ] vcpkg installed and configured
- [ ] SFML, Entt, spdlog, nlohmann-json, fmt installed via vcpkg

## Build Verification

### 1. Clean Build

- [ ] Open Harmony.sln in Visual Studio
- [ ] Select Debug | x64 configuration
- [ ] Clean solution (Build > Clean Solution)
- [ ] Rebuild solution (Build > Rebuild Solution)
- [ ] Verify: 0 errors
- [ ] Note any warnings (expected: none related to our changes)

### 2. Release Build

- [ ] Select Release | x64 configuration
- [ ] Clean solution
- [ ] Rebuild solution
- [ ] Verify: 0 errors

### 3. Sandbox Build

- [ ] Ensure Sandbox project builds successfully
- [ ] Verify: Links with Harmony library correctly

## Functionality Verification

### 1. Basic Engine Test

Create a simple test program or use the Sandbox:

```cpp
#include <Harmony/pch.h>
#include <Harmony/Harmony.h>

int main() {
    Harmony::Utilities::Logger::initialize("test.log");
    
    Harmony::Utilities::Configuration config;
    config.load("configuration.json");
    
    Harmony::Engine engine(config);
    
    // Test 1: Engine creates successfully
    // Expected: No exceptions
    
    // Test 2: getDeltaTime returns float
    float dt = engine.getDeltaTime();
    HARMONY_INFO("Delta time: {}", dt);
    // Expected: Compiles and runs
    
    // Test 3: Engine can start (if configuration valid)
    // engine.start();
    
    Harmony::Utilities::Logger::shutdown();
    return 0;
}
```

- [ ] Test compiles without errors
- [ ] Test runs without crashes
- [ ] Logger outputs correctly

### 2. Scene Entity Test

```cpp
Harmony::Utilities::Configuration entityConfig;
// Set up entity configuration...

// Test: EntityID type works
Harmony::Scenes::EntityID entity = scene.createEntity(entityConfig);
// Expected: Compiles with EntityID type

// Test: Component access
auto& transform = scene.componentReference<Harmony::Components::Transform>(entity);
// Expected: Compiles and runs
```

- [ ] Entity creation works
- [ ] EntityID type accepted by compiler
- [ ] Component access works

### 3. Transform Component Test

```cpp
Harmony::Components::Transform transform(config);

// Test: Setters
transform.setPosition(100.0f, 200.0f);
transform.setRotation(45.0f);
transform.setScale(2.0f, 2.0f);
// Expected: Compiles and runs

// Test: Getters
float x, y;
transform.getPosition(x, y);
float rotation = transform.getRotation();
// Expected: Compiles and runs
```

- [ ] Transform methods work
- [ ] No SFML types in client code
- [ ] Internal transform accessible for rendering

## Compilation Speed Test

### Before (Baseline)

Use an old build or reference:
- Record full rebuild time: ______ seconds
- Record incremental rebuild time: ______ seconds

### After Refactoring

- [ ] Full rebuild time: ______ seconds
- [ ] Incremental rebuild time: ______ seconds
- [ ] Expected: Similar or faster than before

## Header Include Test

Create a test file that only includes Engine.h:

```cpp
#include <Harmony/Engine.h>

// This should compile without requiring SFML headers
void test() {
    // Don't need to use anything, just verify it compiles
}
```

- [ ] Compiles without SFML headers in include path
- [ ] Compiles quickly

## API Compatibility Test

### Delta Time Migration

```cpp
// Old code (should NOT compile):
// sf::Time dt = engine.getDeltaTime();

// New code (should compile):
float dt = engine.getDeltaTime();
```

- [ ] Old code fails to compile (expected)
- [ ] New code compiles successfully

### Entity ID Migration

```cpp
// Old code (should still work but discouraged):
// entt::entity entity = scene.createEntity(config);

// New code (should compile):
Harmony::Scenes::EntityID entity = scene.createEntity(config);
```

- [ ] New EntityID type works
- [ ] Functions accept EntityID

## Potential Issues and Solutions

### Issue: "Cannot find Engine.h"
**Solution:** Verify include paths in project settings

### Issue: "Unresolved external symbol EngineImpl"
**Solution:** Ensure Engine.cpp is included in build

### Issue: "sf::RenderWindow not found in Engine.cpp"
**Solution:** Verify SFML is properly linked (private dependency)

### Issue: "entt::registry not found"
**Solution:** Verify Entt is installed and included in Scene.cpp

### Issue: Linker errors with transform
**Solution:** Ensure Transform.cpp is compiled and linked

## Expected Warnings

The following warnings are acceptable:

- C4251: DLL interface warnings (if building as DLL)
- Level 4 warnings about unused parameters in template code

The following warnings are NOT acceptable and should be fixed:

- Any warnings about undefined symbols
- Any warnings about missing headers
- Deprecation warnings

## Performance Verification

If you have a game/demo application:

- [ ] Load time: Similar to before
- [ ] Frame rate: Same as before
- [ ] Memory usage: Similar to before
- [ ] No new crashes or bugs

## Documentation Verification

- [ ] Read SUMMARY.md
- [ ] Read REFACTORING_NOTES.md
- [ ] Understand API changes in API_REFERENCE.md
- [ ] Review BEFORE_AFTER.md comparisons

## Sign-off

Once all checks pass:

- [ ] All builds successful
- [ ] All tests pass
- [ ] No regressions found
- [ ] Documentation reviewed
- [ ] Performance acceptable

**Verified by:** ________________
**Date:** ________________
**Build configuration:** ________________
**Notes:** ________________

## Next Steps

After successful verification:

1. Merge PR into main branch
2. Update release notes
3. Consider implementing Phase 1 of ARCHITECTURE_ROADMAP.md
4. Plan for remaining Shape class refactoring

## Troubleshooting

If build fails:

1. Check REFACTORING_NOTES.md for API changes
2. Review compiler errors carefully
3. Ensure all new files are included in project
4. Verify vcpkg dependencies are up to date
5. Try clean rebuild
6. Check that Scene.inl is being found by Scene.h

If you encounter issues not covered here, please document them and report back!
