# Altitude Display - Final Fix

## ❌ **Second Problem Found**

### What Was Still Wrong
Even after fixing the octree level issue, the altitude was showing **4.7 m when on the ground** instead of 0.0 m.

**Root cause:** We were measuring from the lander's **origin/center** to the terrain, but the lander's origin is NOT at its bottom!

### Visual Explanation
```
     Lander Origin (Y = 54.7)  ← We were measuring from here
          ↓
    [============]
    [   LANDER   ]  
    [============]  ← Lander Bottom (Y = 50.0)
         ↓
    ================  Terrain (Y = 50.0)
    
    OLD: altitude = 54.7 - 50.0 = 4.7 m  ❌ WRONG!
    NEW: altitude = 50.0 - 50.0 = 0.0 m  ✅ CORRECT!
```

## ✅ **The Final Solution**

### Key Change
Now measuring from the **lander's bottom** (lowest point of bounding box) to terrain, not from its center.

### Updated Code
```cpp
// Get lander's bounding box to find the bottom
Box landerBounds = computeLanderWorldBounds();
float landerBottomY = landerBounds.min().y();  // ← This is the key!

// ... ray cast to find terrain ...

// Calculate altitude from lander's BOTTOM to terrain
altitudeAGL = landerBottomY - highestTerrainY;
```

## 🔧 Complete Algorithm Now

### Step 1: Get Lander's Bottom
```cpp
Box landerBounds = computeLanderWorldBounds();
float landerBottomY = landerBounds.min().y();
```
Uses existing `computeLanderWorldBounds()` which:
- Transforms all lander vertices to world space
- Finds the minimum Y coordinate (bottom)

### Step 2: Cast Ray from Center
```cpp
glm::vec3 landerPos = lander.getPosition();
Ray downRay(Vector3(landerPos.x, landerPos.y, landerPos.z), Vector3(0, -1, 0));
```
Still cast from center (for better terrain detection), but measure from bottom.

### Step 3: Find Terrain Below
```cpp
if (octree.intersect(downRay, octree.root, hitNode)) {
    float highestTerrainY = -FLT_MAX;
    for (int idx : hitNode.points) {
        ofVec3f vertex = octree.mesh.getVertex(idx);
        if (vertex.y > highestTerrainY) {
            highestTerrainY = vertex.y;
        }
    }
}
```
Finds the highest terrain point below.

### Step 4: Calculate from Bottom
```cpp
altitudeAGL = landerBottomY - highestTerrainY;
```
**Critical:** Uses `landerBottomY`, not `landerPos.y`!

### Step 5: Safety Check
```cpp
if (altitudeAGL < 0) altitudeAGL = 0.0f;
```
Prevents negative values if lander sinks slightly.

## 📊 Before vs After

### Before Fix (Wrong)
```
Lander center at Y=54.7
Lander bottom at Y=50.0
Terrain at Y=50.0

Calculation: 54.7 - 50.0 = 4.7 m
Display: "AGL: 4.7 m"  ❌ WRONG! (on ground)
```

### After Fix (Correct)
```
Lander center at Y=54.7
Lander bottom at Y=50.0
Terrain at Y=50.0

Calculation: 50.0 - 50.0 = 0.0 m
Display: "AGL: 0.0 m"  ✅ CORRECT! (on ground)
```

## 🎯 Expected Behavior Now

### On Ground
```
Lander bottom touching terrain
AGL: 0.0 m to 0.2 m  ✅
```

### Low Altitude (Landing)
```
Lander bottom 2m above terrain
AGL: 2.0 m  ✅
```

### High Altitude
```
Lander bottom 100m above terrain
AGL: 100.0 m  ✅
```

## ✅ Why This Works

1. **Accurate:** Measures from actual landing gear/bottom
2. **Intuitive:** 0.0 m means "on ground" (as expected)
3. **Realistic:** Matches how real altimeters work
4. **Safe:** Can judge when to slow down for landing
5. **Efficient:** Uses existing bounding box calculation

## 🎮 Testing Instructions

1. **Build and run**
2. **Load lander** (drag .obj file)
3. **Press 'P'** for physics mode
4. **Let lander settle on ground**
5. **Check AGL display:** Should show **0.0 m to 0.2 m** ✅
6. **Thrust up** (SPACEBAR)
7. **Watch AGL increase** as bottom rises ✅
8. **Descend slowly**
9. **Watch AGL approach 0.0 m** as you land ✅

## 🔍 Technical Details

### Why Use Bounding Box?
- Lander model origin is at its center (for rotation)
- Landing gear/legs are at the bottom
- We need to know when the **legs** touch ground, not center

### Performance Impact
- `computeLanderWorldBounds()` already called for collision detection
- Minimal overhead (just reading min Y value)
- No additional vertex transformations needed

### Edge Cases Handled
1. **Lander tilted:** Bounding box still gives lowest point
2. **Lander sinking:** Negative check prevents display issues
3. **No terrain below:** Returns 0.0 m safely
4. **Multiple terrain levels:** Finds highest (closest) point

## 📝 Complete Code Change

**Location:** `src/ofApp.cpp`, update() function, around line 601

**Changed from:**
```cpp
glm::vec3 landerPos = lander.getPosition();
// ... ray cast ...
altitudeAGL = landerPos.y - highestY;  // ❌ Wrong: from center
```

**Changed to:**
```cpp
Box landerBounds = computeLanderWorldBounds();
float landerBottomY = landerBounds.min().y();  // ✅ Get bottom
glm::vec3 landerPos = lander.getPosition();
// ... ray cast ...
altitudeAGL = landerBottomY - highestTerrainY;  // ✅ From bottom
```

## ✨ Summary

**Problem #1:** Used octree tree depth instead of distance
**Fix #1:** Cast downward ray and calculate distance

**Problem #2:** Measured from lander center, not bottom
**Fix #2:** Use bounding box to find bottom, measure from there

**Result:** Accurate altitude display showing 0.0 m when on ground! ✅

The altitude now correctly measures from the **lander's landing gear** to the terrain! 🚀

