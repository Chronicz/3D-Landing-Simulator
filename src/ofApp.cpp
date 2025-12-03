
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   < Ronald Celino >
//  Date: <11/9/25 - 11/16/25>


#include "ofApp.h"
#include "Util.h"
#include <cfloat>


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
	lastFrameTime = ofGetElapsedTimef();  // Initialize frame time tracking
//	ofSetWindowShape(1024, 768);
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	// can switch between terrains here - just commented out one if not used
	// probably a better way to do this but idk im too lazy...

	//mars.loadModel("geo/mars-low-5x-v2.obj");
	mars.loadModel("geo/moonterrain(enl)_finalproject.obj"); 

	mars.setScaleNormalization(false);

	// Load lander model
	if (lander.loadModel("lander.obj")) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 30, 0);  // Position 30 units above origin
		cout << "Lander loaded. Number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		// Initialize SpaceLander physics
		spaceLander.setup(glm::vec3(0, 30, 0), 1000.0f, 100.0f);
	}
	else {
		cout << "Error: Could not load lander.obj" << endl;
	}

	// create sliders for testing
	//
	gui.setup();
	gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 10));
	bHide = false;

	// added the toggle for timing info 
	gui.add(bTimingInfo.setup("TIming Info", false));

	//  Create Octree for testing.
	//
	// time the octree build
	// chose microseconds to convert back into millis because ofGetElapsedTimeMillis would always just give 0 ms for build and ray-octree intersect times
	float startBuild = ofGetElapsedTimeMicros();
	octree.create(mars.getMesh(0), 20);
	float endBuild = ofGetElapsedTimeMicros();
	//if (bTimingInfo) 
	double build_ms = (endBuild - startBuild) / 1000.0; // convert microseconds to milliseconds (since 1 microsecond = 0.001 milliseconds)
	cout << "Octree build time: " << (build_ms) << " ms" << endl; // print statement for timing info - same for both ray and box intersection tests later below

	cout << "Number of Verts: " << mars.getMesh(0).getNumVertices() << endl;

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));



}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
	// Update SpaceLander controls based on key states
	if (bLanderLoaded && !bLanded) {  // Only update if not landed
		// Main thrust (Space bar)
		spaceLander.setThrust(bThrustKey ? 1.0f : 0.0f);
		
		// Forward/backward (R/F) - tilts main thruster forward/back (affects thrust direction)
		float forwardBack = 0.0f;
		if (bUpKey) forwardBack = 1.0f;
		if (bDownKey) forwardBack = -1.0f;
		spaceLander.setForward(forwardBack);
		
		// Left/right (A/D) - lateral movement
		float lateral = 0.0f;
		if (bLeftKey) lateral = -1.0f;
		if (bRightKey) lateral = 1.0f;
		spaceLander.setLateral(lateral);
		
		// Up/down (W/S) - vertical movement
		float vertical = 0.0f;
		if (bForwardKey) vertical = 1.0f;
		if (bBackwardKey) vertical = -1.0f;
		spaceLander.setVertical(vertical);
		
		// Yaw (Q/E) - rotation around UP axis
		float yaw = 0.0f;
		if (bYawLeftKey) yaw = -1.0f;
		if (bYawRightKey) yaw = 1.0f;
		spaceLander.setYaw(yaw);
		
		// Pitch control (I/K) - nose up/down (RCS rotation)
		float pitch = 0.0f;
		if (bPitchUpKey) pitch = -1.0f;  // I now pitches down
		if (bPitchDownKey) pitch = 1.0f;  // K now pitches up
		spaceLander.setPitch(pitch);
		
		// Roll control (J/L)
		float roll = 0.0f;
		if (bRollLeftKey) roll = 1.0f;   // J now rolls right
		if (bRollRightKey) roll = -1.0f; // L now rolls left
		spaceLander.setRoll(roll);

		// Update physics simulation
		float currentTime = ofGetElapsedTimef();
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
		
		// Sanity check for deltaTime (cap at 0.1 seconds to prevent large jumps)
		if (deltaTime > 0.0f && deltaTime < 0.1f) {
			spaceLander.update(deltaTime);
		} else if (deltaTime >= 0.1f) {
			// If deltaTime is too large, use a fixed small timestep
			spaceLander.update(0.016f);  // ~60fps
		}

		// Update lander model position and rotation from physics
		glm::vec3 landerPos = spaceLander.position;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		
		// Convert quaternion to Euler angles for lander model
		// Note: ofxAssimpModelLoader might use different rotation methods
		// We'll apply rotation via transform matrix in draw()
	}

	// Update lander bounding box to current position
	/* had to update position due to problem in collision resolution - landerbounds would hold initial position when dragged into project and not update as lander moved
	resulting in the ship moving infinitely upwards even with the bounding box not colliding with more than 10 boxes anymore */
	ofVec3f min = lander.getSceneMin() + lander.getPosition();
	ofVec3f max = lander.getSceneMax() + lander.getPosition();

	landerBounds = Box(
		Vector3(min.x, min.y, min.z),
		Vector3(max.x, max.y, max.z)
	);

	// Ray-based altitude detection - cast ray downward from bottom of lander
	// Always update altitude (even when landed) for display purposes
	if (bLanderLoaded) {
		// Get bottom of lander bounding box
		Vector3 boxMin = landerBounds.min();
		Vector3 rayStart = Vector3(boxMin.x() + (landerBounds.max().x() - boxMin.x()) * 0.5f, 
		                           boxMin.y(), 
		                           boxMin.z() + (landerBounds.max().z() - boxMin.z()) * 0.5f);
		
		// Cast ray downward from bottom center of lander
		Ray altitudeRay(rayStart, Vector3(0, -1, 0));
		
		glm::vec3 intersectionPoint;
		glm::vec3 normal;
		float distance;
		
		if (octree.intersectRayMesh(altitudeRay, octree.root, intersectionPoint, normal, distance)) {
			altitude = distance;  // Distance from bottom of lander to terrain
			groundNormal = normal;
		} else {
			altitude = -1.0f;  // No ground detected
		}
	}

	// Free camera movement with arrow keys
	if (bArrowUp || bArrowDown || bArrowLeft || bArrowRight) {
		glm::vec3 pos = cam.getPosition();
		glm::vec3 forward = -glm::normalize(cam.getZAxis());  // Camera forward direction (negative Z is forward)
		
		// Calculate right vector using camera's orientation
		// Try to get X axis directly, or calculate from forward and up
		glm::vec3 right;
		glm::vec3 worldUp = glm::vec3(0, 1, 0);
		
		// Calculate right vector: right = forward × up, then normalize
		right = glm::normalize(glm::cross(forward, worldUp));
		
		// If forward is parallel to world up, use a different reference
		if (glm::length(right) < 0.1f) {
			glm::vec3 worldForward = glm::vec3(0, 0, -1);
			right = glm::normalize(glm::cross(worldForward, worldUp));
		}

		glm::vec3 moveDir(0, 0, 0);

		if (bArrowUp) {
			// Move forward
			moveDir += forward * cameraMoveSpeed;
		}
		if (bArrowDown) {
			// Move backward
			moveDir -= forward * cameraMoveSpeed;
		}
		if (bArrowLeft) {
			// Strafe left
			moveDir -= right * cameraMoveSpeed;
		}
		if (bArrowRight) {
			// Strafe right
			moveDir += right * cameraMoveSpeed;
		}

		// Update camera position
		cam.setPosition(pos + moveDir);
	}

	// Collision detection and resolution with terrain
	if (bLanderLoaded && !bLanded) {  // Only check collisions if not already landed
		colBoxList.clear();
		octree.intersect(landerBounds, octree.root, colBoxList);
		
		bCollisionDetected = (colBoxList.size() > 0);
		
		if (bCollisionDetected) {
			// Check if lander has landed (low velocity and on ground)
			float velocityMagnitude = glm::length(spaceLander.velocity);
			if (velocityMagnitude < 0.5f && altitude < 0.5f) {
				// Lander has landed - make it static
				bLanded = true;
				spaceLander.velocity = glm::vec3(0, 0, 0);
				spaceLander.angularVelocity = glm::vec3(0, 0, 0);
				// Position is already set, just keep it static - no more position corrections
			} else {
				// Collision detected but not fully landed yet - resolve with position correction
				// Find the closest point on terrain to lander
				
				// Cast multiple rays from lander bounding box to find penetration depth
				float minPenetration = FLT_MAX;
				glm::vec3 collisionNormal(0, 1, 0);  // Default to upward
				glm::vec3 collisionPoint;
				
				// Check bottom corners and center of lander bounding box
				Vector3 boxMin = landerBounds.min();
				Vector3 boxMax = landerBounds.max();
				Vector3 boxCenter = landerBounds.center();
				
				// Test points on bottom of bounding box (cast rays upward from below to find terrain)
				Vector3 testPoints[5] = {
					boxCenter,  // Center
					Vector3(boxMin.x(), boxMin.y(), boxMin.z()),  // Bottom corner
					Vector3(boxMax.x(), boxMin.y(), boxMin.z()),  // Bottom corner
					Vector3(boxMin.x(), boxMin.y(), boxMax.z()),  // Bottom corner
					Vector3(boxMax.x(), boxMin.y(), boxMax.z())   // Bottom corner
				};
				
				for (int i = 0; i < 5; i++) {
					// Cast ray downward from above the test point to find terrain
					Vector3 rayStart = testPoints[i] + Vector3(0, 10, 0);  // Start 10 units above
					Ray testRay(rayStart, Vector3(0, -1, 0));
					glm::vec3 hitPoint;
					glm::vec3 hitNormal;
					float hitDistance;
					
					if (octree.intersectRayMesh(testRay, octree.root, hitPoint, hitNormal, hitDistance)) {
						// Check if terrain is above the test point (penetration)
						float terrainHeight = hitPoint.y;
						float testPointHeight = testPoints[i].y();
						
						if (terrainHeight > testPointHeight) {
							float penetration = terrainHeight - testPointHeight;
							if (penetration < minPenetration) {
								minPenetration = penetration;
								collisionNormal = hitNormal;
								collisionPoint = hitPoint;
							}
						}
					}
				}
				
				// If we found a penetration, resolve the collision
				if (minPenetration < FLT_MAX && minPenetration > 0) {
					// Calculate collision resolution
					// Push lander up by penetration depth plus a small margin to prevent sinking
					float pushDistance = minPenetration + 0.01f;
					
					// Ensure normal points upward (away from terrain)
					if (glm::dot(collisionNormal, glm::vec3(0, 1, 0)) < 0) {
						collisionNormal = -collisionNormal;
					}
					
					// Move lander up along the collision normal to resolve penetration
					glm::vec3 pushVector = collisionNormal * pushDistance;
					spaceLander.position += pushVector;
					
					// For landing simulator: stop downward velocity and apply strong damping
					// Calculate velocity component along collision normal
					float velocityAlongNormal = glm::dot(spaceLander.velocity, collisionNormal);
					
					// If moving into the terrain (downward), stop that component
					if (velocityAlongNormal < 0) {
						// Remove velocity component into terrain (no bounce, just stop)
						glm::vec3 velocityIntoTerrain = collisionNormal * velocityAlongNormal;
						spaceLander.velocity -= velocityIntoTerrain;
						
						// Apply strong damping to horizontal velocity on contact (friction)
						glm::vec3 horizontalVelocity = spaceLander.velocity - (collisionNormal * glm::dot(spaceLander.velocity, collisionNormal));
						spaceLander.velocity -= horizontalVelocity * 0.7f;  // Friction damping
					}
					
					// Update lander model position
					lander.setPosition(spaceLander.position.x, spaceLander.position.y, spaceLander.position.z);
				}
			}
		}
	}
	
	// If landed, ensure position stays static (no physics updates)
	if (bLanded) {
		spaceLander.velocity = glm::vec3(0, 0, 0);
		spaceLander.angularVelocity = glm::vec3(0, 0, 0);
		// Position remains at last set value
	}
}
//--------------------------------------------------------------
void ofApp::draw() {

	ofBackground(ofColor::black);

	glDepthMask(false);
	if (!bHide) gui.draw();
	glDepthMask(true);

	cam.begin();
	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		ofPushMatrix();
		ofRotateDeg(180, 1, 0, 0);  // Flip terrain upside down
		mars.drawWireframe();
		ofPopMatrix();
		if (bLanderLoaded) {
			ofPushMatrix();
			// Apply SpaceLander transform (position and rotation)
			glm::mat4 transform = spaceLander.getTransformMatrix();
			ofMultMatrix(transform);
			ofRotateDeg(180, 1, 0, 0);  // Flip lander model upside down (model orientation fix)
			ofVec3f landerPos = lander.getPosition();
			lander.setPosition(0, 0, 0);  // Temporarily set to origin for drawing
			lander.drawWireframe();
			lander.setPosition(landerPos.x, landerPos.y, landerPos.z);  // Restore position
			ofPopMatrix();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		ofPushMatrix();
		ofRotateDeg(180, 1, 0, 0);  // Flip terrain upside down
		mars.drawFaces();
		ofPopMatrix();
		ofMesh mesh;
		if (bLanderLoaded) {
			ofPushMatrix();
			// Apply SpaceLander transform (position and rotation)
			glm::mat4 transform = spaceLander.getTransformMatrix();
			ofMultMatrix(transform);
			ofRotateDeg(180, 1, 0, 0);  // Flip lander model upside down (model orientation fix)
			ofVec3f landerPos = lander.getPosition();
			lander.setPosition(0, 0, 0);  // Temporarily set to origin for drawing
			lander.drawFaces();
			lander.setPosition(landerPos.x, landerPos.y, landerPos.z);  // Restore position
			ofPopMatrix();

			// draw colliding boxes (disabled - no yellow highlights)
			// ofSetColor(ofColor::yellow);
			// for (auto & b : colBoxList) {
			// 	Octree::drawBox(b);
			// }

			if (!bTerrainSelected) drawAxis(lander.getPosition());
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				for (int i = 0; i < lander.getNumMeshes(); i++) {
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
				ofNoFill();
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
		ofPushMatrix();
		ofRotateDeg(180, 1, 0, 0);  // Flip terrain upside down
		mars.drawVertices();
		ofPopMatrix();
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
		ofSetColor(ofColor::white);
		octree.draw(numLevels, 0);
	}

	// if point selected, draw a sphere
	//
	if (pointSelected) {
		ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
		ofVec3f d = p - cam.getPosition();
		ofSetColor(ofColor::lightGreen);
		ofDrawSphere(p, .02 * d.length());
	}

	ofPopMatrix();
	cam.end();

	// Display altitude and collision info on screen
	if (bLanderLoaded) {
		ofSetColor(ofColor::white);
		string altitudeText = "Altitude: ";
		if (altitude >= 0) {
			altitudeText += ofToString(altitude, 2) + " units";
		} else {
			altitudeText += "N/A";
		}
		ofDrawBitmapString(altitudeText, 20, 30);
		
		if (bCollisionDetected) {
			ofSetColor(ofColor::red);
			ofDrawBitmapString("COLLISION DETECTED!", 20, 50);
		}
		
		if (bLanded) {
			ofSetColor(ofColor::green);
			ofDrawBitmapString("LANDED - STATIC", 20, 90);
		}
		
		// Display velocity info
		ofSetColor(ofColor::yellow);
		string velocityText = "Velocity: " + ofToString(glm::length(spaceLander.velocity), 2) + " units/s";
		ofDrawBitmapString(velocityText, 20, 70);
	}
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

	switch (key) {
	case 'B':
	case 'b':
		bDisplayBBoxes = !bDisplayBBoxes;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'H':
	case 'h':
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
	// 'r' and 's' handled below with lander controls
	case 't':
		setCameraTarget();
		break;
	case 'u': // brings lander up if colliding with 10 or more boxes (used for testing collision resolution requirement)
		if (colBoxList.size() >= 10) {
			resolvingCollision = true;
		}
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
	case 'W':
		if (bLanderLoaded) {
			bForwardKey = true;
		} else {
			toggleWireframeMode();
		}
		break;
	case 's':
	case 'S':
		if (bLanderLoaded) {
			bBackwardKey = true;
		} else {
			savePicture();
		}
		break;
	case 'a':
	case 'A':
		if (bLanderLoaded) {
			bLeftKey = true;
		}
		break;
	case 'd':
	case 'D':
		if (bLanderLoaded) {
			bRightKey = true;
		}
		break;
	case 'r':
	case 'R':
		if (bLanderLoaded) {
			bUpKey = true;
		} else {
			cam.reset();
		}
		break;
	case 'f':
	case 'F':
		if (bLanderLoaded) {
			bDownKey = true;
		} else {
			ofToggleFullscreen();
		}
		break;
	case 'q':
	case 'Q':
		if (bLanderLoaded) {
			bYawLeftKey = true;
		}
		break;
	case 'e':
	case 'E':
		if (bLanderLoaded) {
			bYawRightKey = true;
		}
		break;
	case 'i':
	case 'I':
		if (bLanderLoaded) {
			bPitchUpKey = true;
		}
		break;
	case 'k':
	case 'K':
		if (bLanderLoaded) {
			bPitchDownKey = true;
		}
		break;
	case 'j':
	case 'J':
		if (bLanderLoaded) {
			bRollLeftKey = true;
		}
		break;
	case 'l':
	case 'L':
		if (bLanderLoaded) {
			bRollRightKey = true;
		} else {
			bDisplayLeafNodes = !bDisplayLeafNodes;
		}
		break;
	case ' ':
		if (bLanderLoaded) {
			bThrustKey = true;
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
	case OF_KEY_UP:
		bArrowUp = true;
		break;
	case OF_KEY_DOWN:
		bArrowDown = true;
		break;
	case OF_KEY_LEFT:
		bArrowLeft = true;
		break;
	case OF_KEY_RIGHT:
		bArrowRight = true;
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
	case OF_KEY_UP:
		bArrowUp = false;
		break;
	case OF_KEY_DOWN:
		bArrowDown = false;
		break;
	case OF_KEY_LEFT:
		bArrowLeft = false;
		break;
	case OF_KEY_RIGHT:
		bArrowRight = false;
		break;
	case 'w':
	case 'W':
		bForwardKey = false;
		break;
	case 's':
	case 'S':
		bBackwardKey = false;
		break;
	case 'a':
	case 'A':
		bLeftKey = false;
		break;
	case 'd':
	case 'D':
		bRightKey = false;
		break;
	case 'r':
	case 'R':
		bUpKey = false;
		break;
	case 'f':
	case 'F':
		bDownKey = false;
		break;
	case 'q':
	case 'Q':
		bYawLeftKey = false;
		break;
	case 'e':
	case 'E':
		bYawRightKey = false;
		break;
	case 'i':
	case 'I':
		bPitchUpKey = false;
		break;
	case 'k':
	case 'K':
		bPitchDownKey = false;
		break;
	case 'j':
	case 'J':
		bRollLeftKey = false;
		break;
	case 'l':
	case 'L':
		bRollRightKey = false;
		break;
	case ' ':
		bThrustKey = false;
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

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if moving camera, don't allow mouse interaction
//
	if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection
	//
	if (bLanderLoaded) {
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
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
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


	// ray-intersection timing test
	float startSelect = ofGetElapsedTimeMicros();
	pointSelected = octree.intersect(ray, octree.root, selectedNode);
	float endSelect = ofGetElapsedTimeMicros();

	double select_ms = (endSelect - startSelect) / 1000.0; // convert microseconds to milliseconds
	if (bTimingInfo) // only print if toggle enabled
		cout << "Ray-Octree intersection time: " << (select_ms) << " ms" << endl;

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);
	}
	return pointSelected;
}




//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	if (bInDrag) {

		glm::vec3 landerPos = lander.getPosition();

		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();

		// bounding box inserection time test
		//float startIntersect = ofGetElapsedTimeMicros();
		octree.intersect(bounds, octree.root, colBoxList);
		//float endIntersect = ofGetElapsedTimeMicros();

		//double intersect_ms = (endIntersect - startIntersect) / 1000.0; // convert microseconds to milliseconds
		//if (bTimingInfo) // toggle
			//cout << "Bounding Box-Octree intersection time: " << (intersect_ms) << " ms" << endl;
	

		/*if (bounds.overlap(testBox)) {
			cout << "overlap" << endl;
		}
		else {
			cout << "OK" << endl;
		}*/


	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
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

		bLanderLoaded = true;
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
		cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//		lander.setRotation(1, 180, 1, 0, 0);

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
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

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
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}
