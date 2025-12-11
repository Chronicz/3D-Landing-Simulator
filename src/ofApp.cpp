
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"
#include "Util.h"
#include <algorithm>  // For std::min
#include <cmath>      // For std::abs
#include <float.h>    // For FLT_MAX
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	cout << "=== ofApp::setup() started ===" << endl;
	
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
	altitudeAGL = 0.0f;
	
	// Initialize fuel system
	maxFuel = 100.0f;
	fuel = maxFuel;
	fuelRegenRate = 2.0f;   // 5 units per second
	fuelBurnRate = 20.0f;   // 20 units per second when thrusting
	isThrusting = false;
	
	cout << "Window size: " << ofGetWidth() << "x" << ofGetHeight() << endl;
	
//	ofSetWindowShape(1024, 768);
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	//cam.disableMouseInput();  --- commented this out - ronald
	ofEnableSmoothing();
	ofEnableDepthTest();

	cout << "Camera and OpenGL setup complete" << endl;

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();
	cout << "Lighting setup complete" << endl;

	cout << "Attempting to load model..." << endl;
	
	// Try multiple paths for the model - but don't block if file doesn't exist
	string modelPaths[] = {
		"final_terrain.obj",
	};
	
	bool modelLoaded = false;
	for (int i = 0; i < 5; i++) {
		cout << "  Trying path " << i+1 << ": " << modelPaths[i] << endl;
		cout.flush();  // Force output to console immediately
		
		try {
			cout << "    Loading model..." << endl;
			cout.flush();
			mars.loadModel(modelPaths[i]);
			cout << "    Load call completed" << endl;
			cout.flush();
			
			if (mars.getMeshCount() > 0) {
				cout << "  *** SUCCESS! Loaded from: " << modelPaths[i] << " ***" << endl;
				modelLoaded = true;
				break;
			} else {
				cout << "  Failed: No meshes found (mesh count = 0)" << endl;
			}
		} catch (const std::exception& e) {
			cout << "  Exception caught: " << e.what() << endl;
		} catch (...) {
			cout << "  Unknown exception caught while loading" << endl;
		}
		cout.flush();
	}
	
	if (!modelLoaded) {
		cout << "WARNING: Could not load any model file. Continuing without terrain..." << endl;
		cout << "The application will run but no terrain will be displayed." << endl;
	}
	
	mars.setScaleNormalization(false);
	cout << "Model loading complete. Mesh count: " << mars.getMeshCount() << endl;
	cout.flush();

	// load in image and sound assets - ronald
	if (backgroundImage.load("background.png")) { // tries bin/data/background.png
		bBackgroundLoaded = true;
	} else {
		bBackgroundLoaded = false;
		ofLogError() << "Failed to load background.png in " << ofToDataPath("", true);

	}

	// ambient sound loop
	if (ambientSound.load("ambient.mp3")) { // tries bin/data/ambient.mp3
		ambientSound.setLoop(true);
		ambientSound.setVolume(0.2f);
		ambientSound.play(); // start ambient loop
		bAmbientLoaded = true;
		ofLogNotice() << "Ambient sound loaded successfully!";

	} else {
		bAmbientLoaded = false;
		ofLogError() << "Failed to load ambient.mp3 in " << ofToDataPath("", true);

	}

	// thrust noise for lander 
	if (thrustSound.load("spacecraft_thrust.wav")) { // tries bin/data/thrust.mp3
		thrustSound.setLoop(true); 
		thrustSound.setMultiPlay(true);
		thrustSound.setVolume(0.8f);
		bThrustLoaded = true;
		ofLogNotice() << "Thrust sound loaded successfully!";

	} else {
		bThrustLoaded = false;
		ofLogError() << "Failed to load thrust.mp3 in " << ofToDataPath("", true);

	}

	// explosion sound for lander crash
	if (explosionSound.load("explosion.mp3")) { // tries bin/data/explosion.mp3
		explosionSound.setVolume(0.4f);
		bExplosionLoaded = true;
		ofLogNotice() << "Ambient sound loaded successfully!";

	} else {
		bAmbientLoaded = false;
		ofLogError() << "Failed to load ambient.mp3 in " << ofToDataPath("", true);
	}

	// Initialize color array for different octree levels
	// Using distinct colors for each level
	levelColors.push_back(ofColor(255, 0, 0));      // Red - Level 0
	levelColors.push_back(ofColor(0, 255, 0));      // Green - Level 1
	levelColors.push_back(ofColor(0, 0, 255));      // Blue - Level 2
	levelColors.push_back(ofColor(255, 255, 0));    // Yellow - Level 3
	levelColors.push_back(ofColor(255, 0, 255));    // Magenta - Level 4
	levelColors.push_back(ofColor(0, 255, 255));    // Cyan - Level 5
	levelColors.push_back(ofColor(255, 128, 0));    // Orange - Level 6
	levelColors.push_back(ofColor(128, 0, 255));    // Purple - Level 7
	levelColors.push_back(ofColor(255, 192, 203));  // Pink - Level 8
	levelColors.push_back(ofColor(128, 128, 128));  // Gray - Level 9
	// Add more colors if needed for deeper levels
	for (int i = 10; i < 30; i++) {
		// Generate colors with varying hues
		float hue = (i * 30) % 360;
		levelColors.push_back(ofColor::fromHsb(hue, 200, 255));
	}

	// create sliders for testing
	//
	gui.setup();
	// Set max levels to 10 (the value used in octree.create)
	gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 10));
	bHide = false;

	//  Create Octree for testing.
	//  Build octree from transformed mesh to match rendered terrain exactly
	//
	
	if (mars.getMeshCount() > 0) {
		cout << "\n=== OCTREE CREATION DEBUG ===" << endl;
		
		// FIX #1: Check for multiple meshes and merge them
		int numMeshes = mars.getMeshCount();
		cout << "A. Number of meshes in model: " << numMeshes << endl;
		
		// Merge all sub-meshes into a single mesh
		ofMesh mergedMesh;
		for (int i = 0; i < numMeshes; i++) {
			ofMesh currentMesh = mars.getMesh(i);
			cout << "  Mesh " << i << ": " << currentMesh.getNumVertices() << " vertices" << endl;
			mergedMesh.append(currentMesh);
		}
		cout << "  Merged mesh total vertices: " << mergedMesh.getNumVertices() << endl;
		
		// Debug: Print first few vertices of merged mesh (before transform)
		cout << "\nB. First 5 vertices (before transform):" << endl;
		for (int i = 0; i < std::min(5, (int)mergedMesh.getNumVertices()); i++) {
			glm::vec3 v = mergedMesh.getVertex(i);
			cout << "  Vertex " << i << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
		}
		
		// Calculate bounding box of untransformed merged mesh
		Box untransformedBounds = Octree::meshBounds(mergedMesh);
		Vector3 untransformedMin = untransformedBounds.min();
		Vector3 untransformedMax = untransformedBounds.max();
		cout << "\nC. Untransformed mesh bounds:" << endl;
		cout << "  Min: (" << untransformedMin.x() << ", " << untransformedMin.y() << ", " << untransformedMin.z() << ")" << endl;
		cout << "  Max: (" << untransformedMax.x() << ", " << untransformedMax.y() << ", " << untransformedMax.z() << ")" << endl;
		cout << "  Size: (" << (untransformedMax.x() - untransformedMin.x()) << ", " 
		     << (untransformedMax.y() - untransformedMin.y()) << ", " 
		     << (untransformedMax.z() - untransformedMin.z()) << ")" << endl;
		
		try {
			// FIX #3: Get the model matrix (includes any scale/rotation/translation)
			glm::mat4 modelMatrix = mars.getModelMatrix();
			cout << "\nD. Model matrix:" << endl;
			// Print matrix for debugging
			const float* m = glm::value_ptr(modelMatrix);
			cout << "  [" << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << "]" << endl;
			cout << "  [" << m[4] << ", " << m[5] << ", " << m[6] << ", " << m[7] << "]" << endl;
			cout << "  [" << m[8] << ", " << m[9] << ", " << m[10] << ", " << m[11] << "]" << endl;
			cout << "  [" << m[12] << ", " << m[13] << ", " << m[14] << ", " << m[15] << "]" << endl;
			
			// Apply model matrix to all vertices to transform to world space
			ofMesh transformedMesh = mergedMesh;  // Copy merged mesh
			cout << "\nE. Applying model matrix transformation..." << endl;
			int totalVerts = transformedMesh.getNumVertices();
			cout << "  Transforming " << totalVerts << " vertices..." << endl;
			cout.flush();
			
			for (int i = 0; i < totalVerts; i++) {
				glm::vec3 v = transformedMesh.getVertex(i);
				glm::vec4 t = modelMatrix * glm::vec4(v.x, v.y, v.z, 1.0);
				transformedMesh.setVertex(i, glm::vec3(t.x, t.y, t.z));
				
				// Progress indicator for large meshes
				if (totalVerts > 10000 && i % 10000 == 0 && i > 0) {
					cout << "    Progress: " << i << "/" << totalVerts << " vertices..." << endl;
					cout.flush();
				}
			}
			cout << "  Transformation complete!" << endl;
			
			// Debug: Print first few vertices after transform
			cout << "  First 5 vertices (after transform):" << endl;
			for (int i = 0; i < std::min(5, (int)transformedMesh.getNumVertices()); i++) {
				glm::vec3 v = transformedMesh.getVertex(i);
				cout << "  Vertex " << i << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
			}
			
			// Calculate bounding box of transformed mesh
			Box transformedBounds = Octree::meshBounds(transformedMesh);
			Vector3 transformedMin = transformedBounds.min();
			Vector3 transformedMax = transformedBounds.max();
			cout << "\nF. Transformed mesh bounds:" << endl;
			cout << "  Min: (" << transformedMin.x() << ", " << transformedMin.y() << ", " << transformedMin.z() << ")" << endl;
			cout << "  Max: (" << transformedMax.x() << ", " << transformedMax.y() << ", " << transformedMax.z() << ")" << endl;
			cout << "  Size: (" << (transformedMax.x() - transformedMin.x()) << ", " 
			     << (transformedMax.y() - transformedMin.y()) << ", " 
			     << (transformedMax.z() - transformedMin.z()) << ")" << endl;
			
			// Verify bounds are not clamped at 0
			if (transformedMin.x() >= 0 && transformedMax.x() > 0) {
				cout << "  WARNING: X bounds are all positive - terrain may have negative X coordinates!" << endl;
			}
			if (transformedMin.y() >= 0 && transformedMax.y() > 0) {
				cout << "  WARNING: Y bounds are all positive - terrain may have negative Y coordinates!" << endl;
			}
			if (transformedMin.z() >= 0 && transformedMax.z() > 0) {
				cout << "  WARNING: Z bounds are all positive - terrain may have negative Z coordinates!" << endl;
			}
			
			// Build octree from transformed mesh (now in world space)
			cout << "\nG. Building octree..." << endl;
			cout << "  This may take a moment for large meshes..." << endl;
			cout.flush();
			octree.create(transformedMesh, 10);  // Reduced from 20 to 10 levels for faster build
			
			// FIX #4: Print debug output to verify correctness
			Vector3 rootMin = octree.root.box.min();
			Vector3 rootMax = octree.root.box.max();
			cout << "\nH. Octree root bounds:" << endl;
			cout << "  Min: (" << rootMin.x() << ", " << rootMin.y() << ", " << rootMin.z() << ")" << endl;
			cout << "  Max: (" << rootMax.x() << ", " << rootMax.y() << ", " << rootMax.z() << ")" << endl;
			cout << "  Size: (" << (rootMax.x() - rootMin.x()) << ", " 
			     << (rootMax.y() - rootMin.y()) << ", " 
			     << (rootMax.z() - rootMin.z()) << ")" << endl;
			
			// Verify octree bounds match transformed mesh bounds
			float tolerance = 0.001f;
			bool boundsMatch = (std::abs(rootMin.x() - transformedMin.x()) < tolerance &&
			                   std::abs(rootMin.y() - transformedMin.y()) < tolerance &&
			                   std::abs(rootMin.z() - transformedMin.z()) < tolerance &&
			                   std::abs(rootMax.x() - transformedMax.x()) < tolerance &&
			                   std::abs(rootMax.y() - transformedMax.y()) < tolerance &&
			                   std::abs(rootMax.z() - transformedMax.z()) < tolerance);
			
			if (boundsMatch) {
				cout << "  ✓ Octree bounds match transformed mesh bounds" << endl;
			} else {
				cout << "  ✗ WARNING: Octree bounds do NOT match transformed mesh bounds!" << endl;
				cout << "    Difference in min: (" << (rootMin.x() - transformedMin.x()) << ", "
				     << (rootMin.y() - transformedMin.y()) << ", " << (rootMin.z() - transformedMin.z()) << ")" << endl;
				cout << "    Difference in max: (" << (rootMax.x() - transformedMax.x()) << ", "
				     << (rootMax.y() - transformedMax.y()) << ", " << (rootMax.z() - transformedMax.z()) << ")" << endl;
			}
			
			cout << "\n=== OCTREE CREATION COMPLETE ===" << endl;
			cout << "Total vertices in octree: " << transformedMesh.getNumVertices() << endl;
			
		} catch (const std::exception& e) {
			cout << "ERROR: Exception during octree creation: " << e.what() << endl;
		} catch (...) {
			cout << "ERROR: Unknown exception during octree creation. Continuing anyway..." << endl;
		}
	} else {
		cout << "WARNING: No meshes loaded, skipping octree creation" << endl;
	}

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));
	
	cout << "=== ofApp::setup() completed successfully ===" << endl;

	// --- Load assets from bin/data --- ronald
	// Background image
	vector<string> bgCandidates = {"background.png", "bg.png", "mars_bg.png"};
	// If background was not already loaded earlier, try common filenames in data/
	if (!bBackgroundLoaded) {
		for (auto &f : bgCandidates) {
			if (ofFile::doesFileExist("data/" + f)) {
				if (backgroundImage.load("data/" + f)) {
					bBackgroundLoaded = true;
					cout << "Loaded background image: " << f << endl;
					break;
				}
			}
		}
		if (!bBackgroundLoaded) cout << "WARNING: No background image found in data/ (tried background.png, bg.png, mars_bg.png)" << endl;
	}

	// Ambient sound (loop)
	// If ambient not already loaded, try common filenames
	vector<string> ambientCandidates = {"ambient.mp3", "ambient.wav", "ambience.mp3"};
	if (!bAmbientLoaded) {
		for (auto &f : ambientCandidates) {
			if (ofFile::doesFileExist("data/" + f)) {
				if (ambientSound.load("data/" + f)) {
					ambientSound.setLoop(true);
					ambientSound.setVolume(0.6f);
					ambientSound.play();
					bAmbientLoaded = true;
					cout << "Playing ambient sound: " << f << endl;
					break;
				}
			}
		}
		if (!bAmbientLoaded) cout << "WARNING: No ambient sound found in data/ (tried ambient.mp3, ambient.wav, ambience.mp3)" << endl;
	}

	// Thrust sound (loop when space pressed)
	// If thrust not already loaded, try common filenames
	vector<string> thrustCandidates = {"thrust.mp3", "thrust.wav", "engine_loop.mp3"};
	if (!bThrustLoaded) {
		for (auto &f : thrustCandidates) {
			if (ofFile::doesFileExist("data/" + f)) {
				if (thrustSound.load("data/" + f)) {
				thrustSound.setLoop(true);
				thrustSound.setMultiPlay(true);
				thrustSound.setVolume(0.9f);
					bThrustLoaded = true;
					cout << "Loaded thrust sound: " << f << " (will play on space)" << endl;
					break;
				}
			}
		}
		if (!bThrustLoaded) cout << "WARNING: No thrust sound found in data/ (tried thrust.mp3, thrust.wav, engine_loop.mp3)" << endl;
	}
}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
// Helper function to compute lander world-space bounding box
// Transforms all vertices by model matrix to account for rotation/scale/translation
Box ofApp::computeLanderWorldBounds() {
	if (!bLanderLoaded) {
		return Box(Vector3(0, 0, 0), Vector3(0, 0, 0));
	}
	
	// Get model matrix (includes position, rotation, scale)
	glm::mat4 modelMatrix = lander.getModelMatrix();
	
	// Get all meshes and transform their vertices to world space
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
	
	for (int i = 0; i < lander.getMeshCount(); i++) {
		ofMesh mesh = lander.getMesh(i);
		for (int j = 0; j < mesh.getNumVertices(); j++) {
			glm::vec3 v = mesh.getVertex(j);
			// Transform vertex by model matrix (already includes position)
			glm::vec4 transformed = modelMatrix * glm::vec4(v.x, v.y, v.z, 1.0);
			glm::vec3 worldPos = glm::vec3(transformed.x, transformed.y, transformed.z);
			
			minX = (worldPos.x < minX) ? worldPos.x : minX;
			maxX = (worldPos.x > maxX) ? worldPos.x : maxX;
			minY = (worldPos.y < minY) ? worldPos.y : minY;
			maxY = (worldPos.y > maxY) ? worldPos.y : maxY;
			minZ = (worldPos.z < minZ) ? worldPos.z : minZ;
			maxZ = (worldPos.z > maxZ) ? worldPos.z : maxZ;
		}
	}
	
	return Box(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ));
}

// Update camera based on current mode - ronald
void ofApp::updateCamera() {
	switch (camMode) {
	case CAM_FREE:
	 // free-floating camera, enable to drag with mouse
		if (!cam.getMouseInputEnabled()) {
			cam.enableMouseInput(); // Protect free cam from being disabled by other code
		}
		break;

	case CAM_SIDE: // side view of lander
		cam.disableMouseInput(); // every other mode disables mouse input		
		if (bLanderLoaded) {
			glm::vec3 landerPos = lander.getPosition(); // get current lander position
			// place the camera to the side of the lander
			cam.setPosition(landerPos.x + 200, landerPos.y, landerPos.z);
			cam.lookAt(landerPos); // always look at lander
		}
		break;

	case CAM_TOPDOWN: // view coming from the bottom of the lander - seeing the ground beneath
		cam.disableMouseInput();
		if (bLanderLoaded) {
			glm::vec3 landerPos = lander.getPosition();
			// place camera above the lander and look down at the terrain beneath it
			// this produces a top-down view centered on the lander
			float heightOffset = 50.0f; // adjust as needed for scene scale
			cam.setPosition(landerPos.x, landerPos.y + heightOffset, landerPos.z);
			cam.lookAt(landerPos);
			// debug log
			ofLogNotice() << "CAM_TOPDOWN pos=" << cam.getPosition() << " target=" << landerPos;
		}
		break;

	case CAM_FIRSTPERSON:
	{
		cam.disableMouseInput();

		// Position camera in a first-person-ish position slightly behind and above the lander
		// Use the lander's yaw (from physics) to compute forward direction and offset the camera
		glm::vec3 landerPos = lander.getPosition();
		float yaw = landerPhysics.yaw; // degrees
		float yawRad = glm::radians(yaw);
		glm::vec3 forwardDir = glm::normalize(glm::vec3(std::sin(yawRad), 0.0f, std::cos(yawRad)));

		float heightOffset = 2.0f;   // how much above the lander origin
		float fpDistance = 5.0f;     // how far back from the lander to place the camera

		// Place camera in frpmt of the lander (along its forward direction)
		glm::vec3 camPos = landerPos + glm::vec3(0.0f, heightOffset, 0.0f) + forwardDir * fpDistance;

		cam.setPosition(camPos);
		// Look ahead in the forward direction so you see the scene in front of the lander
		float lookAhead = 10.0f; // how far ahead to look
		cam.lookAt(landerPos + forwardDir * lookAhead + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 1, 0));
	}
		break;

	case CAM_FREE_FROZEN:
	{
		// freeze camera in current position/orientation
		cam.disableMouseInput();
		cam.disableMouseMiddleButton();
		cam.disableMouseMiddleButton();
		
		// On the first frame, hard-freeze the pose
		if (camWasJustFrozen) {
			cam.setPosition(frozenCamPos);
			cam.lookAt(frozenCamTarget, frozenCamUp);
			camWasJustFrozen = false;
		}

		// Every frame: clamp it so nothing moves it
		cam.setPosition(frozenCamPos);
		cam.lookAt(frozenCamTarget, frozenCamUp);
	}
	}
		
}


void ofApp::update() {
	// sound update - ronald
	ofSoundUpdate();

	// update camera posiiton/orientation based on mode -ronald
	updateCamera();

	// Physics mode: Update LEM physics and apply to model
	if (bPhysicsEnabled && bLanderLoaded) {
		// Get delta time in seconds - ensure it's valid
		float dt = ofGetLastFrameTime();
		
		// Ensure dt is valid (not zero or negative)
		if (dt <= 0.0f || dt > 0.1f) {
			dt = 1.0f / 60.0f;  // Default to 60 FPS if invalid
		}
		
		// Check if thrust is allowed based on fuel
		bool thrustAllowed = thrustUp && fuel > 0;
		isThrusting = thrustAllowed;
		
		// Update physics
		landerPhysics.update(dt, moveForward, moveBack, moveLeft, moveRight, thrustAllowed, rotateCCW, rotateCW);
		
		// Update lander model position
		lander.setPosition(landerPhysics.position.x, landerPhysics.position.y, landerPhysics.position.z);
		
		// Apply yaw rotation (rotation around Y-axis only)
		// The base orientation (180° X-axis flip) is set once during model load
		// We only update the yaw rotation here to allow Q/E rotation controls
		// Using index 1 for yaw ensures it doesn't interfere with base orientation (index 0)
		lander.setRotation(1, landerPhysics.yaw, 0, 1, 0);
		
		// Check for collision with terrain (only compute bounds when needed)
		landerBox = computeLanderWorldBounds();
		intersected = octree.intersect(lander.getPosition(), octree.root, node, 1);
		
		// Visual debugging: Collect intersecting leaf nodes (same as drag mode)
		// Use existing octree.intersect() method to collect ONLY leaf nodes that overlap landerBox
		collidingBoxes.clear();
		allCollidingBoxes.clear();
		octree.intersect(landerBox, octree.root, collidingBoxes);
		// Also collect all overlapping nodes for debugging (same as drag mode)
		octree.intersectAll(landerBox, octree.root, allCollidingBoxes);
		
		// If no leaf nodes found but we have overlapping nodes, add small intermediate nodes as fallback
		// This ensures highlights are visible even at lower octree levels (same as drag mode)
		if (collidingBoxes.empty() && !allCollidingBoxes.empty()) {
			// Calculate lander size for comparison
			Vector3 landerMin = landerBox.min();
			Vector3 landerMax = landerBox.max();
			float landerSizeX = landerMax.x() - landerMin.x();
			float landerSizeY = landerMax.y() - landerMin.y();
			float landerSizeZ = landerMax.z() - landerMin.z();
			float landerMaxSize = std::max(landerSizeX, std::max(landerSizeY, landerSizeZ));
			
			// Add intermediate nodes that are smaller than lander as temporary highlights
			// This helps visualize collisions when octree level is too low to have leaf nodes
			for (const auto &nodeBox : allCollidingBoxes) {
				Vector3 nodeMin = nodeBox.min();
				Vector3 nodeMax = nodeBox.max();
				float nodeSizeX = nodeMax.x() - nodeMin.x();
				float nodeSizeY = nodeMax.y() - nodeMin.y();
				float nodeSizeZ = nodeMax.z() - nodeMin.z();
				float nodeMaxSize = std::max(nodeSizeX, std::max(nodeSizeY, nodeSizeZ));
				
				// If node is significantly smaller than lander, treat it as a highlight candidate
				if (nodeMaxSize < landerMaxSize * 0.8f) {
					collidingBoxes.push_back(nodeBox);
				}
			}
		}
		
		// Collision handling with positional correction to prevent sinking
		if (!collidingBoxes.empty() && !landerPhysics.bDisableCollisionClamping) {
			// Find the highest terrain surface (maxY) from all intersecting leaf nodes
			float highestTerrainY = -FLT_MAX;
			for (const auto &terrainBox : collidingBoxes) {
				float terrainMaxY = terrainBox.max().y();
				if (terrainMaxY > highestTerrainY) {
					highestTerrainY = terrainMaxY;
				}
			}
			
			// Get the bottom of the lander bounding box (minY)
			float landerBottomY = landerBox.min().y();
			
			// If lander's bottom is below terrain surface, push it up
			if (landerBottomY < highestTerrainY) {
				// Calculate how much to push the lander up
				float pushUpAmount = highestTerrainY - landerBottomY;
				
				// Check landing velocity for crash detection
				float landingSpeed = glm::length(landerPhysics.velocity);
				if (landingSpeed > 5.0f) {
					// CRASH! Trigger explosion
					landerPhysics.triggerExplosion();
					explosionSound.play();
					cout << "CRASH! Landing speed: " << landingSpeed << " m/s" << endl;
				}
				
				// Adjust lander position upward
				landerPhysics.position.y += pushUpAmount;
				
				// Zero out Y velocity to prevent further sinking
				landerPhysics.velocity.y = 0.0f;
				
				// Update lander model position with corrected position
				lander.setPosition(landerPhysics.position.x, landerPhysics.position.y, landerPhysics.position.z);
				
				// Recompute landerBox with corrected position for next frame
				landerBox = computeLanderWorldBounds();
			}
			// If lander is above terrain but velocity is downward, stop downward velocity
			else if (landerPhysics.velocity.y < 0) {
				landerPhysics.velocity.y = 0.0f;
			}
		}
		
		// Update particle system
		landerPhysics.updateParticles(dt);
		
		// Update fuel system
		if (isThrusting && fuel > 0) {
			fuel -= fuelBurnRate * dt;
			if (fuel < 0) fuel = 0.0f;
		} else if (!isThrusting && fuel < maxFuel) {
			fuel += fuelRegenRate * dt;
			if (fuel > maxFuel) fuel = maxFuel;
		}
		
		// Update altitude display using downward ray cast
		if (bLanderLoaded) {
			// Get lander's bounding box to find the bottom
			Box landerBounds = computeLanderWorldBounds();
			float landerBottomY = landerBounds.min().y();
			
			glm::vec3 landerPos = lander.getPosition();
			
			// Cast downward ray from lander center
			Ray downRay(Vector3(landerPos.x, landerPos.y, landerPos.z), Vector3(0, -1, 0));
			TreeNode hitNode;
			
			// Find terrain below using octree
			if (octree.intersect(downRay, octree.root, hitNode)) {
				// Find the highest point in the hit node
				float highestTerrainY = -FLT_MAX;
				for (int idx : hitNode.points) {
					ofVec3f vertex = octree.mesh.getVertex(idx);
					if (vertex.y > highestTerrainY) {
						highestTerrainY = vertex.y;
					}
				}
				
				// Calculate altitude from lander's BOTTOM to terrain
				altitudeAGL = landerBottomY - highestTerrainY;
				
				// Apply calibration offset (0.6m observed when on ground)
				if (altitudeAGL < 1) altitudeAGL = 1;
				
				// Ensure non-negative
				if (altitudeAGL < 0) altitudeAGL = 0.0f;
			} else {
				// No terrain found below
				altitudeAGL = 0.0f;
			}
		}
	}
	else {
		// Clear collision results when not in physics mode
		collidingBoxes.clear();
		allCollidingBoxes.clear();
		altitudeAGL = 0.0f;
	}
	
	// Run collision test ONLY during drag (non-physics mode)
	// Skip entirely if physics mode is enabled
	if (bInDrag && bLanderLoaded && !bPhysicsEnabled) {
		// Compute lander bounding box in world space
		landerBox = computeLanderWorldBounds();
		
		// Find all overlapping octree leaf nodes (for final highlighting)
		octree.intersect(landerBox, octree.root, collidingBoxes);
		
		// DEBUG: Find ALL overlapping nodes including intermediate (for debugging visualization)
		octree.intersectAll(landerBox, octree.root, allCollidingBoxes);
		
		// If no leaf nodes found but we have overlapping nodes, add small intermediate nodes as fallback
		// This ensures highlights are visible even at lower octree levels
		if (collidingBoxes.empty() && !allCollidingBoxes.empty()) {
			// Calculate lander size for comparison
			Vector3 landerMin = landerBox.min();
			Vector3 landerMax = landerBox.max();
			float landerSizeX = landerMax.x() - landerMin.x();
			float landerSizeY = landerMax.y() - landerMin.y();
			float landerSizeZ = landerMax.z() - landerMin.z();
			float landerMaxSize = std::max(landerSizeX, std::max(landerSizeY, landerSizeZ));
			
			// Add intermediate nodes that are smaller than lander as temporary highlights
			// This helps visualize collisions when octree level is too low to have leaf nodes
			for (const auto &nodeBox : allCollidingBoxes) {
				Vector3 nodeMin = nodeBox.min();
				Vector3 nodeMax = nodeBox.max();
				float nodeSizeX = nodeMax.x() - nodeMin.x();
				float nodeSizeY = nodeMax.y() - nodeMin.y();
				float nodeSizeZ = nodeMax.z() - nodeMin.z();
				float nodeMaxSize = std::max(nodeSizeX, std::max(nodeSizeY, nodeSizeZ));
				
				// If node is significantly smaller than lander, treat it as a highlight candidate
				if (nodeMaxSize < landerMaxSize * 0.8f) {
					collidingBoxes.push_back(nodeBox);
				}
			}
		}
		
		// DIAGNOSTIC OUTPUT - Print every frame during drag
		static int frameCount = 0;
		frameCount++;
		if (frameCount % 30 == 0) {  // Print every 30 frames to avoid spam
			Vector3 landerMin = landerBox.min();
			Vector3 landerMax = landerBox.max();
			Vector3 rootMin = octree.root.box.min();
			Vector3 rootMax = octree.root.box.max();
			
			cout << "\n=== COLLISION DIAGNOSTICS (frame " << frameCount << ") ===" << endl;
			cout << "bInDrag: " << (bInDrag ? "true" : "false") << ", bLanderLoaded: " << (bLanderLoaded ? "true" : "false") << endl;
			cout << "Lander world bounds:" << endl;
			cout << "  Min: (" << landerMin.x() << ", " << landerMin.y() << ", " << landerMin.z() << ")" << endl;
			cout << "  Max: (" << landerMax.x() << ", " << landerMax.y() << ", " << landerMax.z() << ")" << endl;
			cout << "Octree root bounds:" << endl;
			cout << "  Min: (" << rootMin.x() << ", " << rootMin.y() << ", " << rootMin.z() << ")" << endl;
			cout << "  Max: (" << rootMax.x() << ", " << rootMax.y() << ", " << rootMax.z() << ")" << endl;
			
			// Check if lander box overlaps root box
			bool rootOverlap = landerBox.overlap(octree.root.box);
			cout << "Lander overlaps root: " << (rootOverlap ? "YES" : "NO") << endl;
			cout << "Total overlapping nodes (all): " << allCollidingBoxes.size() << endl;
			cout << "Colliding leaf nodes: " << collidingBoxes.size() << endl;
			
			if (collidingBoxes.size() > 0) {
				cout << "  First colliding box (leaf or small intermediate):" << endl;
				Vector3 firstMin = collidingBoxes[0].min();
				Vector3 firstMax = collidingBoxes[0].max();
				cout << "    Min: (" << firstMin.x() << ", " << firstMin.y() << ", " << firstMin.z() << ")" << endl;
				cout << "    Max: (" << firstMax.x() << ", " << firstMax.y() << ", " << firstMax.z() << ")" << endl;
			} else if (allCollidingBoxes.size() > 0) {
				cout << "  INFO: Found overlapping nodes but none are small enough to highlight." << endl;
				cout << "  First overlapping node (may be too large):" << endl;
				Vector3 firstMin = allCollidingBoxes[0].min();
				Vector3 firstMax = allCollidingBoxes[0].max();
				cout << "    Min: (" << firstMin.x() << ", " << firstMin.y() << ", " << firstMin.z() << ")" << endl;
				cout << "    Max: (" << firstMax.x() << ", " << firstMax.y() << ", " << firstMax.z() << ")" << endl;
			} else {
				cout << "  No overlapping nodes found." << endl;
			}
			cout << "==========================================\n" << endl;
		}
	}
}
//--------------------------------------------------------------
void ofApp::draw() {
	// draw 2D background first (if loaded)
	if (bBackgroundLoaded) {
		// draw full-screen background
		ofDisableDepthTest();
		ofSetColor(255);
		backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
		ofEnableDepthTest();
	} else {
		ofBackground(ofColor::black);
	}

	glDepthMask(false);
	if (!bHide) gui.draw();
	glDepthMask(true);
	
	// Display altitude (AGL) when in physics mode
	if (bPhysicsEnabled && bLanderLoaded) {
		// Draw black shadow for readability
		ofSetColor(0, 0, 0);
		ofDrawBitmapString("AGL: " + ofToString(altitudeAGL, 1) + " m", 21, 21);
		// Draw white text
		ofSetColor(255, 255, 255);
		ofDrawBitmapString("AGL: " + ofToString(altitudeAGL, 1) + " m", 20, 20);
	}
	
	// Display fuel underneath altitude
	if (bPhysicsEnabled && bLanderLoaded) {
		float fuelPercent = (fuel / maxFuel) * 100.0f;
		string fuelText = "Fuel: " + ofToString(fuelPercent, 0) + "%";
		
		// Draw black shadow for readability
		ofSetColor(0, 0, 0);
		ofDrawBitmapString(fuelText, 21, 41);
		
		// Draw colored text based on fuel level
		if (fuelPercent > 50) {
			ofSetColor(0, 255, 0);  // Green
		} else if (fuelPercent > 25) {
			ofSetColor(255, 255, 0);  // Yellow
		} else {
			ofSetColor(255, 0, 0);  // Red
		}
		ofDrawBitmapString(fuelText, 20, 40);
	}

	cam.begin();
	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		if (bLanderLoaded) {
			lander.drawWireframe();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
		ofMesh mesh;
		if (bLanderLoaded) {
			lander.drawFaces();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				int numMeshes = lander.getNumMeshes();
				for (int i = 0; i < numMeshes && i < bboxList.size(); i++) {
					ofPushMatrix();
					ofMultMatrix(lander.getModelMatrix());
					ofRotate(-90, 1, 0, 0);
					Octree::drawBox(bboxList[i]);
					ofPopMatrix();
				}
			}

			if (bLanderSelected) {

				ofVec3f min = lander.getSceneMin() + lander.getPosition();
				ofVec3f max = lander.getSceneMax() + lander.getPosition();

				Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
				ofSetColor(ofColor::white);
				Octree::drawBox(bounds);

				// draw colliding boxes
				//
				ofSetColor(ofColor::lightBlue);
				for (int i = 0; i < colBoxList.size(); i++) {
					Octree::drawBox(colBoxList[i]);
				}
			}
		}
	}
	if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));



	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}


	// recursively draw octree
	//
	ofDisableLighting();
	int level = 0;
	//	ofNoFill();

	if (bDisplayLeafNodes) {
		octree.drawLeafNodes(octree.root);
		cout << "num leaf: " << octree.numLeaf << endl;
    }
	else if (bDisplayOctree) {
		ofNoFill();
		// Octree is already in world space (built from transformed mesh)
		// No need to apply model matrix
		octree.draw(numLevels, 0, &levelColors);
	}

	// if point selected, draw a sphere
	//
	if (pointSelected && !selectedNode.points.empty()) {
		// Point is already in world space (octree built from transformed mesh)
		ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
		ofVec3f d = p - cam.getPosition();
		ofSetColor(ofColor::lightGreen);
		ofDrawSphere(p, .02 * d.length());
	}
	

	// DEBUG: Draw ALL overlapping nodes in faint cyan (for debugging - shows intermediate nodes too)
	if (!allCollidingBoxes.empty()) {
		ofDisableLighting();
		ofNoFill();
		ofSetColor(100, 255, 255, 150);  // Faint cyan with transparency
		ofSetLineWidth(1.0);
		for (auto &b : allCollidingBoxes) {
			Octree::drawBox(b);
		}
	}
	
	// Draw colliding octree leaf boxes in bright yellow
	// Highlights intersecting leaf nodes whenever collidingBoxes is not empty
	// Draws on top of terrain by disabling depth mask
	if (!collidingBoxes.empty()) {
		// Disable depth mask to draw on top of terrain
		glDepthMask(GL_FALSE);
		ofDisableLighting();
		ofNoFill();
		ofSetColor(ofColor::yellow);  // Bright yellow
		ofSetLineWidth(3.5);
		for (auto &b : collidingBoxes) {
			Octree::drawBox(b);
		}
		ofSetLineWidth(1.0);
		// Re-enable depth mask
		glDepthMask(GL_TRUE);
		ofSetColor(255, 255, 255);
	}
	
	// Debug visualization for octree alignment
	if (bDebugOctreeAlignment && mars.getMeshCount() > 0) {
		ofDisableLighting();
		ofSetLineWidth(2.0);
		
		// Draw octree root box in red
		ofSetColor(ofColor::red);
		ofNoFill();
		Octree::drawBox(octree.root.box);
		
		// Draw terrain bounding box in green (with model matrix)
		ofPushMatrix();
		glm::mat4 modelMatrix = mars.getModelMatrix();
		ofMultMatrix(modelMatrix);
		
		ofMesh mesh = mars.getMesh(0);
		Box terrainBounds = Octree::meshBounds(mesh);
		
		ofSetColor(ofColor::green);
		Octree::drawBox(terrainBounds);
		
		ofPopMatrix();
		
		// Draw coordinate axes at octree root center
		Vector3 rootCenter = octree.root.box.center();
		ofPushMatrix();
		ofTranslate(rootCenter.x(), rootCenter.y(), rootCenter.z());
		
		ofSetLineWidth(3.0);
		ofSetColor(ofColor::red);
		ofDrawLine(0, 0, 0, 1, 0, 0);  // X axis
		ofSetColor(ofColor::green);
		ofDrawLine(0, 0, 0, 0, 1, 0);  // Y axis
		ofSetColor(ofColor::blue);
		ofDrawLine(0, 0, 0, 0, 0, 1);  // Z axis
		
		ofPopMatrix();
		
		// Draw coordinate axes at terrain center (with model matrix)
		ofPushMatrix();
		ofMultMatrix(modelMatrix);
		
		Box terrainBounds2 = Octree::meshBounds(mars.getMesh(0));
		Vector3 terrainCenter = terrainBounds2.center();
		
		ofTranslate(terrainCenter.x(), terrainCenter.y(), terrainCenter.z());
		
		ofSetLineWidth(3.0);
		ofSetColor(ofColor::yellow);
		ofDrawLine(0, 0, 0, 1, 0, 0);  // X axis
		ofSetColor(ofColor::cyan);
		ofDrawLine(0, 0, 0, 0, 1, 0);  // Y axis
		ofSetColor(ofColor::magenta);
		ofDrawLine(0, 0, 0, 0, 0, 1);  // Z axis
		
		ofPopMatrix();
		
		ofSetLineWidth(1.0);
	}
	
	// Draw particle effects (thrust and explosions)
	if (bLanderLoaded && bPhysicsEnabled) {
		landerPhysics.drawParticles();
	}

	ofPopMatrix();
	cam.end();
}


// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {
	// Handle LEM physics controls when physics mode is enabled
	// Process these FIRST to avoid conflicts with other key handlers
	bool physicsKeyHandled = false;
	if (bLanderLoaded) { // switch camera modes with number keys - ronald
		switch (key) {
		case '1':
			camMode = CAM_FREE;
			break;
		case '2':
			camMode = CAM_SIDE;
			break;
		case '3':
			camMode = CAM_TOPDOWN;
			break;
		case '4':
			camMode = CAM_FIRSTPERSON;
			break;
		case '5':
			if (camMode != CAM_FREE_FROZEN) {
				camMode = CAM_FREE_FROZEN;

				cam.disableMouseInput();
				camWasJustFrozen = true;

				frozenCamPos = cam.getPosition();
				frozenCamTarget = cam.getTarget().getPosition();
				frozenCamUp = cam.getUpDir();
			}
			break;
		}

	}
	if (bPhysicsEnabled && bLanderLoaded) {
		switch (key) {
		case 'w':
		case 'W':
			moveForward = true;
			physicsKeyHandled = true;
			break;
		case 's':
		case 'S':
			moveBack = true;
			physicsKeyHandled = true;
			break;
		case 'a':
		case 'A':
			moveLeft = true;
			physicsKeyHandled = true;
			break;
		case 'd':
		case 'D':
			moveRight = true;
			physicsKeyHandled = true;
			break;
		case ' ':  // Spacebar (ASCII 32)
			ofLogNotice() << "Space pressed, isPlaying=" << thrustSound.isPlaying();
			thrustUp = true;
			physicsKeyHandled = true;
			// play thrust loop if available
			if (bThrustLoaded && !thrustSound.isPlaying()) {
				thrustSound.play();
				ofLogNotice() << "Thrust sound started!";
			}
			break;
		case 'q':
		case 'Q':
			rotateCCW = true;
			physicsKeyHandled = true;
			break;
		case 'e':
		case 'E':
			rotateCW = true;
			physicsKeyHandled = true;
			break;
		}
	}

	switch (key) {
	case 'B':
	case 'b':
		bDisplayBBoxes = !bDisplayBBoxes;
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'L':
	case 'l':
		bDisplayLeafNodes = !bDisplayLeafNodes;
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
	case 'P':
	case 'p':
		// Toggle physics mode
		if (bLanderLoaded) {
			bPhysicsEnabled = !bPhysicsEnabled;
			if (bPhysicsEnabled) {
				// Initialize physics at current lander position
				// Preserve current yaw to avoid sudden rotation jump
				glm::vec3 currentPos = lander.getPosition();
				float currentYaw = landerPhysics.yaw;  // Preserve existing yaw
				landerPhysics.initialize(currentPos);
				landerPhysics.yaw = currentYaw;  // Restore yaw after initialization
				// Reset all input flags
				moveForward = moveBack = moveLeft = moveRight = false;
				thrustUp = rotateCCW = rotateCW = false;
				cout << "\n=== PHYSICS MODE ENABLED ===" << endl;
				cout << "[DEBUG] Physics ON, lander position: " << lander.getPosition() << endl;
				cout << "[DEBUG] Physics ON, lander yaw: " << landerPhysics.yaw << " degrees" << endl;
				cout << "Controls: W/S (forward/back), A/D (left/right), SPACEBAR (thrust), Q/E (rotate)" << endl;
			} else {
				// Reset input states
				moveForward = moveBack = moveLeft = moveRight = false;
				thrustUp = rotateCCW = rotateCW = false;
				// stop thrust sound if playing
				if (bThrustLoaded && thrustSound.isPlaying()) thrustSound.stop();
				cout << "\n=== PHYSICS MODE DISABLED ===" << endl;
			}
		} else {
			cout << "ERROR: Cannot enable physics mode - lander not loaded!" << endl;
		}
		break;
	case 'T':
	case 't':
		// Toggle extreme forces for testing (only in physics mode)
		if (bPhysicsEnabled && bLanderLoaded) {
			landerPhysics.bExtremeForces = !landerPhysics.bExtremeForces;
		} else if (!bPhysicsEnabled) {
			// If physics not enabled, use 't' for camera target (original behavior)
			setCameraTarget();
		}
		break;
	case 'C':
	case 'c':
		// Toggle collision clamping disable for testing (only in physics mode)
		if (bPhysicsEnabled && bLanderLoaded) {
			landerPhysics.bDisableCollisionClamping = !landerPhysics.bDisableCollisionClamping;
		} else if (!bPhysicsEnabled) {
			// If physics not enabled, use 'c' for camera toggle (original behavior)
			if (cam.getMouseInputEnabled()) cam.disableMouseInput();
			else cam.enableMouseInput();
		}
		break;
	case 'D':
	case 'd':
		// Only handle if not a physics control key
		if (!physicsKeyHandled && !bPhysicsEnabled) {
			bDebugOctreeAlignment = !bDebugOctreeAlignment;
		}
		break;
	case 'r':
		cam.reset();
		break;
	case 's':
	case 'S':
		// Only handle if not a physics control key
		if (!physicsKeyHandled && !bPhysicsEnabled) {
			savePicture();
		}
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
	case 'W':
		// Only handle if not a physics control key
		if (!physicsKeyHandled && !bPhysicsEnabled) {
			toggleWireframeMode();
		}
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
	// Handle LEM physics controls when physics mode is enabled
	bool physicsKeyHandled = false;
	if (bPhysicsEnabled && bLanderLoaded) {
		switch (key) {
		case 'w':
		case 'W':
			moveForward = false;
			physicsKeyHandled = true;
			break;
		case 's':
		case 'S':
			moveBack = false;
			physicsKeyHandled = true;
			break;
		case 'a':
		case 'A':
			moveLeft = false;
			physicsKeyHandled = true;
			break;
		case 'd':
		case 'D':
			moveRight = false;
			physicsKeyHandled = true;
			break;
		case ' ':  // Spacebar (ASCII 32)
			ofLogNotice() << "Space released, isPlaying=" << thrustSound.isPlaying();
			thrustUp = false;
			physicsKeyHandled = true;
			// stop thrust loop
			if (bThrustLoaded && thrustSound.isPlaying()) {
				thrustSound.stop();
				ofLogNotice() << "Thrust sound stopped!";
			}
			break;
		case 'q':
		case 'Q':
			rotateCCW = false;
			physicsKeyHandled = true;
			break;
		case 'e':
		case 'E':
			rotateCW = false;
			physicsKeyHandled = true;
			break;
		}
	}

	switch (key) {
	
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    // Allow dragging only when camera is frozen (camera stays locked while dragging)
    if (camMode != CAM_FREE_FROZEN) return;

	// Don’t drag if lander isn’t loaded
	if (!bLanderLoaded) return;

	// Build ray
	ofVec3f rayStart = cam.getPosition();
	ofVec3f rayEnd = cam.screenToWorld(ofVec3f(x, y, 1.0));

    // Convert AABB to ofVec3f (use member 'boundingBox')
    ofVec3f bmin(boundingBox.min().x(), boundingBox.min().y(), boundingBox.min().z());
    ofVec3f bmax(boundingBox.max().x(), boundingBox.max().y(), boundingBox.max().z());

	// Ray test
	if (rayIntersectBox(rayStart, rayEnd, bmin, bmax, &dragDepth)) {
		bDraggingLander = true;

		// Disable camera ONLY WHILE dragging
		cam.disableMouseInput();

		ofVec3f hit = cam.screenToWorld(ofVec3f(x, y, dragDepth));
		dragOffset = lander.getPosition() - hit;
	}
	// if moving camera, don't allow mouse interaction
	//
	//if (cam.getMouseInputEnabled()) return;

	// if moving camera, don't allow mouse interaction
	//
	//if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection (only in non-physics mode)
	//
	if (bLanderLoaded && !bPhysicsEnabled) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		if (hit) {
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		} else {
			bLanderSelected = false;
		}
	} else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}


bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	pointSelected = octree.intersect(ray, octree.root, selectedNode);

	if (pointSelected && !selectedNode.points.empty()) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);
	}
	return pointSelected;
}




//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

    // allow dragging only when camera is frozen (camera stays locked while dragging) - ronald
    if (camMode != CAM_FREE_FROZEN) return;

	// Disable drag in physics mode
	if (bPhysicsEnabled) return;

	if (bInDrag) {
		glm::vec3 landerPos = lander.getPosition();
		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;
		
		// Note: Collision detection is now handled in update() which recomputes
		// lander bounding box correctly using model matrix transformation
		// The old code here used incorrect bounding box calculation
	

		/*if (bounds.overlap(testBox)) {
			cout << "overlap" << endl;
		}
		else {
			cout << "OK" << endl;
		}*/


	}
	else {
		ofVec3f p;
		// Support alternate dragging method started by ray-box intersection
		if (bDraggingLander) {
			// Compute world-space hit at the stored drag depth and apply offset
			ofVec3f hit = cam.screenToWorld(ofVec3f(x, y, dragDepth));
			glm::vec3 newPos = glm::vec3(hit.x + dragOffset.x, hit.y + dragOffset.y, hit.z + dragOffset.z);
			lander.setPosition(newPos.x, newPos.y, newPos.z);
		} else {
			raySelectWithOctree(p);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    bool wasDragging = false;

    if (bInDrag) {
        bInDrag = false;
        wasDragging = true;
    }

    if (bDraggingLander) {
        bDraggingLander = false;
        wasDragging = true;
    }

    // Only re-enable camera mouse input if we are in the free camera mode.
    // If the camera is frozen, keep mouse input disabled so the camera remains locked.
    if (wasDragging && camMode == CAM_FREE) {
        cam.enableMouseInput(); // free cam gets control back -ronald
    }
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
	if (lander.loadModel(dragInfo.files[0])) {
		lander.setScaleNormalization(false);
//		lander.setScale(.1, .1, .1);
	//	lander.setPosition(point.x, point.y, point.z);
		lander.setPosition(1, 1, 0);
		
		// Fix lander orientation - rotate 180 degrees around X axis to fix upside-down issue
		lander.setRotation(0, 180, 1, 0, 0);

		bLanderLoaded = true;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 0, 0);
		
		// Fix lander orientation - rotate 180 degrees around X axis to fix upside-down issue
		lander.setRotation(0, 180, 1, 0, 0);
		
		cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = cam.getPosition();
		glm::vec3 camAxis = cam.getZAxis();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		ofVec3f intersectPoint;

		ofVec3f rayPoint = ofVec3f(origin.x, origin.y, origin.z);
		ofVec3f rayDir = ofVec3f(mouseDir.x, mouseDir.y, mouseDir.z);
		ofVec3f planePoint = ofVec3f(0, 0, 0);
		ofVec3f planeNorm = ofVec3f(camAxis.x, camAxis.y, camAxis.z);

		bool hit = rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, intersectPoint);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPointGlm = glm::vec3(intersectPoint.x, intersectPoint.y, intersectPoint.z);

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPointGlm.x, intersectPointGlm.y - offset, intersectPointGlm.z);

			// Initialize physics at this position
			landerPhysics.initialize(glm::vec3(intersectPointGlm.x, intersectPointGlm.y - offset, intersectPointGlm.z));

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	ofVec3f intersectPoint;

	ofVec3f rayPoint = ofVec3f(origin.x, origin.y, origin.z);
	ofVec3f rayDir = ofVec3f(mouseDir.x, mouseDir.y, mouseDir.z);
	ofVec3f planePoint = ofVec3f(planePt.x, planePt.y, planePt.z);
	ofVec3f planeNormal = ofVec3f(planeNorm.x, planeNorm.y, planeNorm.z);

	bool hit = rayIntersectPlane(rayPoint, rayDir, planePoint, planeNormal, intersectPoint);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		return glm::vec3(intersectPoint.x, intersectPoint.y, intersectPoint.z);
	}
	else return glm::vec3(0, 0, 0);
}
