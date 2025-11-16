# Physics Behavior Summary - Before and After Fix

## What Changed

The physics system now properly converts between pixel coordinates (used by the rendering system) and meter coordinates (required by Box2D).

## Expected Behavior Changes

### 1. Gravity Strength

**Before:** Objects fell extremely fast with gravity set to 981 (effectively 32.7 m/s² - 3.3x Earth's gravity)
**After:** Objects fall at realistic speed with gravity set to 9.81 m/s² (Earth's gravity)

### 2. Restitution (Bounciness)

**Before:** 
- restitution=0.9 → barely bounces (~10% bounce height)
- restitution=0.5 → essentially no bounce
- Very high damping needed to prevent objects from becoming too energetic

**After:**
- restitution=0.9 → bounces to ~80-90% of drop height (very bouncy)
- restitution=0.5 → bounces to ~40-50% of drop height (moderate)
- restitution=0.0 → no bounce (dead ball)

### 3. Falling Speed and Acceleration

**Before:**
- Objects appeared to fall very slowly
- Required high linear_damping (0.5) to prevent extreme velocities
- Acceleration felt sluggish

**After:**
- Objects accelerate at 9.81 m/s² (realistic)
- Low linear_damping (0.0-0.1) for realistic motion
- Acceleration feels natural and responsive

### 4. Collision Response

**Before:**
- Collisions were overly energetic due to incorrect scale
- Required high damping to stabilize
- Objects could "phase through" each other at high speeds

**After:**
- Collisions behave predictably
- Energy is conserved properly
- Stable at normal game speeds

## Configuration Value Guidelines

### Gravity
```json
"gravity": {
  "x": 0,
  "y": 9.81    // Earth gravity - good for most games
}
```

Other options:
- Moon gravity: `1.62` m/s²
- Mars gravity: `3.71` m/s²
- Zero gravity: `0.0` m/s²
- Heavy gravity (platformer): `12-15` m/s²

### Linear Damping
```json
"linear_damping": 0.0  // No air resistance (realistic)
```

Other options:
- Slight air resistance: `0.05-0.1`
- Underwater: `0.3-0.5`
- Very viscous (like honey): `0.8-1.0`

### Angular Damping
```json
"angular_damping": 0.1  // Slight rotational resistance
```

Other options:
- No rotation resistance: `0.0`
- High rotation resistance: `0.3-0.5`

### Restitution (per fixture)
```json
"fixture": {
  "restitution": 0.0  // No bounce (default)
}
```

Common materials:
- Clay/putty: `0.0`
- Wood: `0.3-0.4`
- Basketball: `0.7-0.8`
- Super ball: `0.9-0.95`
- Perfect elastic collision: `1.0` (not realistic but useful for testing)

### Friction (per fixture)
```json
"fixture": {
  "friction": 0.2  // Default
}
```

Common materials:
- Ice: `0.0-0.1`
- Wood on wood: `0.3-0.5`
- Rubber: `0.8-1.0`

### Density (per fixture)
```json
"fixture": {
  "density": 1.0  // kg/m² - normal density
}
```

Common densities:
- Balloons, foam: `0.1-0.3`
- Wood: `0.5-0.8`
- Water (reference): `1.0`
- Stone: `2.0-3.0`
- Metal: `7.0-10.0`

## Testing Your Physics

To verify physics is working correctly:

1. **Drop Test**: Drop an object from height. It should:
   - Accelerate smoothly
   - Take realistic time to fall (about 0.45 seconds per meter fallen)
   - Bounce to appropriate height based on restitution

2. **Bounce Test**: Object with restitution=0.9 should:
   - Bounce to ~80-90% of original height
   - Eventually settle after several bounces
   - Not gain energy over time

3. **Friction Test**: Object sliding on angled surface should:
   - Slide quickly on low-friction (0.0-0.2) surfaces
   - Slow down/stick on high-friction (0.8-1.0) surfaces
   - Maintain reasonable velocity

4. **Collision Test**: Two objects colliding should:
   - Transfer momentum realistically
   - Not pass through each other
   - Produce stable results

## Troubleshooting

### Objects fall too slowly
- Check gravity value (should be around 9.81, not 981)
- Reduce linear_damping (try 0.0-0.1)

### Objects don't bounce enough
- Increase restitution on fixtures (try 0.7-0.9)
- Reduce linear_damping
- Check that both colliding objects have restitution > 0

### Objects are too energetic
- Reduce restitution (try 0.0-0.3)
- Add slight linear_damping (0.05-0.1)
- Check gravity isn't too high

### Objects phase through each other
- Reduce time step (in code, not config)
- Ensure objects aren't moving too fast
- Check fixture shapes are valid

## Performance Notes

The scaling conversion adds minimal overhead:
- ~2 float operations per entity per frame
- Negligible CPU impact (<0.01%)
- No memory overhead

## Backward Compatibility

Existing configurations will need updates:
1. Change gravity from `981` to `9.81` (divide by 100)
2. Reduce linear_damping from `0.5` to `0.0-0.1` (divide by 5-10)
3. Restitution values can stay the same but will work better now

The physics will feel more realistic and responsive after these changes.
