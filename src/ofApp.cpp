
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


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	bWireframe = true;  // Default to wireframe mode for debugging
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
	
	// Initialize camera movement controls
	bArrowUp = false;
	bArrowDown = false;
	bArrowLeft = false;
	bArrowRight = false;
	bMoveUp = false;
	bMoveDown = false;
	cameraMoveSpeed = 0.5f;  // Movement speed per frame
	
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
	
	// Setup the ofLight object as bright directional light from above
	light.setDirectional();
	light.setOrientation(ofVec3f(0, -1, 0));  // Pointing down
	light.setDiffuseColor(ofColor(255, 255, 255));
	light.setAmbientColor(ofColor(255, 255, 255));  // Maximum ambient
	light.enable();

	// can switch between terrains here - just commented out one if not used
	// probably a better way to do this but idk im too lazy...

	//mars.loadModel("geo/mars-low-5x-v2.obj");
	mars.loadModel("geo/moonterrain(enl)_finalproject.obj"); 

	mars.setScaleNormalization(false);

	// Load Lander from objects folder - positioned statically above terrain for lighting test
	lander.loadModel("objects/lander.obj");
	lander.setScaleNormalization(false);
	lander.setPosition(0, 50, 0);  // Hovering above terrain at y=50
	bLanderLoaded = true;
	
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
	// Lander is static for lighting test - no position updates
	// Keep lander at fixed position above terrain
	if (bLanderLoaded) {
		lander.setPosition(0, 50, 0);  // Keep lander static at y=50
		
		// Update bounding box for static lander
		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		landerBounds = Box(
			Vector3(min.x, min.y, min.z),
			Vector3(max.x, max.y, max.z)
		);
	}

	// Camera movement using arrow keys
	if (bArrowUp || bArrowDown || bArrowLeft || bArrowRight || bMoveUp || bMoveDown) {
		// Get camera's current orientation vectors
		ofVec3f forward = cam.getZAxis();  // Forward direction (where camera is looking)
		ofVec3f right = cam.getXAxis();    // Right direction
		ofVec3f up = cam.getYAxis();       // Up direction
		
		// Normalize vectors
		forward.normalize();
		right.normalize();
		up.normalize();
		
		// Calculate movement direction
		ofVec3f moveDir(0, 0, 0);
		
		if (bArrowUp) {
			// Move forward (in the direction camera is looking)
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
		if (bMoveUp) {
			// Move up vertically
			moveDir += up * cameraMoveSpeed;
		}
		if (bMoveDown) {
			// Move down vertically
			moveDir -= up * cameraMoveSpeed;
		}
		
		// Get current camera position and apply movement
		ofVec3f currentPos = cam.getPosition();
		cam.setPosition(currentPos + moveDir);
	}

	// Lander is static - no collision resolution needed for lighting test
	// Disabled collision resolution to keep lander static
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
		// Flip terrain upside down - rotate 180 degrees around X axis
		ofPushMatrix();
		ofRotate(180, 1, 0, 0);
		mars.drawWireframe();
		ofPopMatrix();
		// Draw static lander in wireframe mode - flipped upside down
		if (bLanderLoaded) {
			ofPushMatrix();
			ofTranslate(0, 50, 0);  // Move to lander position
			ofRotate(180, 1, 0, 0);  // Flip lander upside down (rotates around current position)
			lander.setPosition(0, 0, 0);  // Draw at origin since we've translated
			lander.drawWireframe();
			ofPopMatrix();
			lander.setPosition(0, 50, 0);  // Reset position
			drawAxis(ofVec3f(0, 50, 0));
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		// Enable lighting - simple global illumination system
		ofEnableLighting();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		
		// Set up directional light from above - pointing straight down
		float lightDir[] = { 0.0, -1.0, 0.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
		
		// Update ofLight
		light.setDirectional();
		light.setOrientation(ofVec3f(0, -1, 0));
		light.enable();
		
		// Set materials to maximum brightness - ensures everything is visible
		// High ambient = no dark areas, everything is lit
		float mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float mat_shininess[] = { 0.0f };
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		
		// Enable color material
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		
		// Flip terrain upside down - rotate 180 degrees around X axis
		ofPushMatrix();
		ofRotate(180, 1, 0, 0);
		mars.drawFaces();
		ofPopMatrix();
		// Draw static lander for lighting test - always visible, flipped upside down
		if (bLanderLoaded) {
			// Ensure lander is at fixed position
			ofPushMatrix();
			ofTranslate(0, 50, 0);  // Move to lander position
			ofRotate(180, 1, 0, 0);  // Flip lander upside down (rotates around current position)
			lander.setPosition(0, 0, 0);  // Draw at origin since we've translated
			lander.drawFaces();
			ofPopMatrix();
			lander.setPosition(0, 50, 0);  // Reset position
			
			// Draw axis at lander position for reference
			drawAxis(ofVec3f(0, 50, 0));
		}
	}
	if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));



	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		// Flip terrain upside down - rotate 180 degrees around X axis
		ofPushMatrix();
		ofRotate(180, 1, 0, 0);
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
	case 'r':
		cam.reset();
		break;
	case 's':
		savePicture();
		break;
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
		toggleWireframeMode();
		break;
	case 'Q':
	case 'q':
		bMoveUp = true;
		break;
	case 'E':
	case 'e':
		bMoveDown = true;
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

	switch (key) {
	case 'Q':
	case 'q':
		bMoveUp = false;
		break;
	case 'E':
	case 'e':
		bMoveDown = false;
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

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if moving camera, don't allow mouse interaction
//
	if (cam.getMouseInputEnabled()) return;

	// Lander is static - disable mouse interaction for lighting test
	// Just do terrain selection
	ofVec3f p;
	raySelectWithOctree(p);
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

	// Lander is static - no dragging allowed for lighting test
	// Just do terrain selection
	ofVec3f p;
	raySelectWithOctree(p);
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
// setup simple global lighting - high ambient for visibility without shadows
//
void ofApp::initLightingAndMaterials() {

	// Simple global lighting system - high ambient makes everything visible
	// No shadows, just bright global illumination
	
	// Set very high ambient material so objects are always visible
	static float mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static float mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static float mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // No specular highlights
	static float mat_shininess[] = { 0.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	// Enable color material so colors work properly
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Setup light with very high ambient and moderate diffuse
	// This ensures everything is well-lit
	static float light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Maximum ambient
	static float light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	static float light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	// Directional light from above - pointing straight down
	static float position[] = { 0.0, -1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	// Very high global ambient - this is the key to making everything visible
	static float lmodel_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Maximum global ambient
	static float lmodel_twoside[] = { GL_TRUE };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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
