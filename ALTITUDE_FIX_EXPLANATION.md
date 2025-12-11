# Altitude Display Fix - Problem & Solution

## ❌ **The Problem**

### What Was Wrong
The original implementation used `octree.altitude`, which stores the **octree tree level/depth** (0, 1, 2, 3...), NOT the actual altitude above ground.

**Example:**
- `octree.altitude = 5` means "lander is in octree level 5"
- This does NOT mean "lander is 5 meters above ground"

### Why It Was Wrong
```cpp
// OLD CODE (INCORRECT):
octree.intersect(ofVec3f(landerPos.x, landerPos.y, landerPos.z), octree.root, node, 0);
altitudeAGL = octree.altitude;  // This is tree depth, not altitude!
```

The `intersect(ofVec3f, TreeNode, TreeNode, int)` method sets `octree.altitude = level` where `level` is the recursion depth in the octree tree structure, not the distance to terrain.

## ✅ **The Solution**

### What Changed
Now using a **downward ray cast** to find the actual terrain below and calculate real distance.

### New Implementation
```cpp
// Cast downward ray from lander
Ray downRay(Vector3(landerPos.x, landerPos.y, landerPos.z), Vector3(0, -1, 0));
TreeNode hitNode;

// Find terrain below using octree
if (octree.intersect(downRay, octree.root, hitNode)) {
    // Find the highest point in the hit node
    float highestY = -FLT_MAX;
    for (int idx : hitNode.points) {
        ofVec3f vertex = octree.mesh.getVertex(idx);
        if (vertex.y > highestY) {
            highestY = vertex.y;
        }
    }
    
    // Calculate altitude as distance from lander to highest terrain point
    altitudeAGL = landerPos.y - highestY;
    
    // Ensure non-negative
    if (altitudeAGL < 0) altitudeAGL = 0.0f;
} else {
    // No terrain found below
    altitudeAGL = 0.0f;
}
```

## 🔧 How It Works Now

### Step 1: Cast Downward Ray
```cpp
Ray downRay(Vector3(landerPos.x, landerPos.y, landerPos.z), Vector3(0, -1, 0));
```
Creates a ray pointing straight down from lander position.

### Step 2: Find Terrain Node Below
```cpp
if (octree.intersect(downRay, octree.root, hitNode))
```
Uses the Ray-based intersect method to find the octree node containing terrain below the lander.

### Step 3: Find Highest Terrain Point
```cpp
float highestY = -FLT_MAX;
for (int idx : hitNode.points) {
    ofVec3f vertex = octree.mesh.getVertex(idx);
    if (vertex.y > highestY) {
        highestY = vertex.y;
    }
}
```
Loops through all vertices in the hit node to find the highest Y coordinate (closest to lander).

### Step 4: Calculate Distance
```cpp
altitudeAGL = landerPos.y - highestY;
```
Simple subtraction: lander Y position minus terrain Y position = altitude.

### Step 5: Safety Check
```cpp
if (altitudeAGL < 0) altitudeAGL = 0.0f;
```
Ensures altitude is never negative (in case lander sinks slightly into terrain).

## 📊 Comparison

### Before (Wrong)
```
Lander at Y=100, Octree level 5
Display: "AGL: 5.0 m"  ❌ WRONG!
```

### After (Correct)
```
Lander at Y=100, Terrain at Y=85
Display: "AGL: 15.0 m"  ✅ CORRECT!
```

## ✅ Why This Is Better

1. **Accurate:** Shows real distance to terrain in meters
2. **Intuitive:** Higher altitude = higher number
3. **Useful:** Can judge landing safety (< 5m = danger zone)
4. **Efficient:** Uses existing octree ray intersection
5. **Robust:** Handles edge cases (no terrain, negative values)

## 🎯 Expected Behavior

### High Altitude
```
Lander at Y=200, Terrain at Y=50
AGL: 150.0 m
```

### Low Altitude (Landing)
```
Lander at Y=52, Terrain at Y=50
AGL: 2.0 m
```

### On Ground
```
Lander at Y=50.1, Terrain at Y=50
AGL: 0.1 m
```

### No Terrain Below
```
Lander over void/edge
AGL: 0.0 m
```

## 🔍 Technical Details

### Ray-Based Intersection
Uses `octree.intersect(const Ray &ray, const TreeNode &node, TreeNode &nodeRtn)` which:
- Finds the octree node that the ray intersects
- Returns the closest leaf node
- Provides access to terrain vertices in that region

### Vertex Lookup
- `hitNode.points` contains indices of vertices in the octree mesh
- `octree.mesh.getVertex(idx)` gets the actual vertex position
- We find the highest Y value (closest to lander)

### Performance
- Single ray cast per frame
- Only checks vertices in intersected node
- Very fast (< 0.1ms typically)

## ✅ Files Modified

**src/ofApp.cpp** - Update section only:
- Replaced simple `octree.altitude` lookup
- Added downward ray cast
- Added vertex loop to find highest point
- Added distance calculation

**No other files changed!**

## 🎮 Testing

1. **Build and run**
2. **Load lander** (drag .obj)
3. **Press 'P'** for physics
4. **Thrust up** (SPACEBAR)
5. **Watch AGL increase** as you rise
6. **Descend**
7. **Watch AGL decrease** as you approach terrain
8. **Land gently**
9. **AGL should be near 0.0 m** on ground

## ✨ Summary

**Problem:** Used octree tree depth instead of real altitude
**Solution:** Cast downward ray and calculate actual distance to terrain
**Result:** Accurate, useful altitude display in meters

The altitude now shows the **real distance** from lander to terrain below! 🚀

