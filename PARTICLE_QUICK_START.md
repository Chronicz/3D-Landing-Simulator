# Particle System - Quick Start Guide

## 🚀 What Was Added

3 new files with particle system:
- `src/Particle.h` - Individual particle
- `src/ParticleEmitter.h` - Particle emitter
- `src/ParticleEmitter.cpp` - Emitter implementation

Integrated into existing Lander class with minimal changes.

## ✅ Files Modified

1. **src/Lander.h** - Added emitter member + 3 methods
2. **src/Lander.cpp** - Added emitter logic
3. **src/ofApp.cpp** - Added crash detection + particle update/draw
4. **3D-Landing-Simulator.vcxproj** - Added new files to project
5. **3D-Landing-Simulator.vcxproj.filters** - Added filters for VS

## 🎮 How to Test

### Test 1: Thrust Particles
1. Run application
2. Load lander (drag .obj file)
3. Press **'P'** (enable physics)
4. Press **SPACEBAR** (thrust)
5. **See:** Orange particles shooting downward

### Test 2: Explosion
1. Enable physics (**'P'**)
2. Let lander fall from height
3. Hit ground fast (> 5 m/s)
4. **See:** Radial explosion burst

### Test 3: Safe Landing
1. Enable physics (**'P'**)
2. Use thrust to slow down
3. Land gently (< 5 m/s)
4. **See:** No explosion

## 📋 Controls

- **P** - Toggle physics mode
- **SPACEBAR** - Thrust (emits particles)
- **W/S** - Forward/Back
- **A/D** - Left/Right
- **Q/E** - Rotate
- **1-5** - Camera modes

## 🔧 Key Features

✅ **Thrust Exhaust**
- Automatic when SPACEBAR pressed
- Cone-shaped emission
- Orange/yellow color
- Stops when thrust released

✅ **Crash Explosion**
- Triggers at > 5 m/s landing speed
- Radial burst (200 particles)
- Bright orange/red
- Affected by gravity

✅ **Automatic Management**
- Particles fade out naturally
- Memory efficient (particle pooling)
- Follows lander position

## 🎯 What Wasn't Changed

- ✅ Octree build
- ✅ Terrain loading
- ✅ Physics calculations
- ✅ Collision detection
- ✅ Camera system
- ✅ Lighting
- ✅ All existing controls

## 📝 Console Output

When crash occurs:
```
CRASH! Landing speed: 8.5 m/s
```

## 🐛 If Something's Wrong

**No particles visible?**
- Check physics is enabled ('P' key)
- Check you're pressing SPACEBAR
- Check lander is loaded

**No explosion?**
- Must land at > 5 m/s
- Must have collision with terrain
- Check console for "CRASH!" message

**Won't compile?**
- Clean solution
- Rebuild
- Check all 3 new files are in src/ folder

## ✨ Summary

Particle system is:
- ✅ Fully implemented
- ✅ Integrated into Lander
- ✅ Ready to compile and run
- ✅ No breaking changes
- ✅ Minimal code additions

Just build and run!

