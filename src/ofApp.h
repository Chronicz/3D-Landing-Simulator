#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include "Lander.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent2(ofDragInfo dragInfo);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		bool raySelectWithOctree(ofVec3f &pointRet);
		glm::vec3 getMousePointOnPlane(glm::vec3 p , glm::vec3 n);
		Box computeLanderWorldBounds();  // Helper to compute lander bounding box in world space

		//--ronald -- camera stuff
		void updateCamera();

		ofEasyCam cam;
		enum CamMode {
			CAM_FREE,
			CAM_SIDE,
			CAM_TOPDOWN,
			CAM_FIRSTPERSON,
			CAM_FREE_FROZEN
		};
		CamMode camMode = CAM_FREE;

		glm::vec3 frozenCamPos;
		glm::vec3 frozenCamTarget;
		glm::vec3 frozenCamUp;
		bool camWasJustFrozen = false;

		ofxAssimpModelLoader mars, lander;
		ofLight light;
		Box boundingBox, landerBounds;
		Box testBox;
		vector<Box> colBoxList;
		Box landerBox;                     // Updated when dragging lander
		vector<Box> collidingBoxes;        // highlight list for AABB collision visualization (leaf nodes only)
		vector<Box> allCollidingBoxes;     // debug: all overlapping nodes including intermediate
		bool bLanderSelected = false;
		Octree octree;
		TreeNode node;
		TreeNode selectedNode;
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;
		bool intersected = false;

		// --ronald -- dragging lander
		bool bDraggingLander = false;
		float dragDepth = 0.0f;
		glm::vec3 dragOffset; // offset between grab point and model origin


		ofxIntSlider numLevels;
		ofxPanel gui;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bHide;
		bool pointSelected = false;
		bool bDisplayLeafNodes = false;
		bool bDisplayOctree = false;
	bool bDisplayBBoxes = false;
	bool bDebugOctreeAlignment = false;  // Debug visualization toggle
	bool bDrawCollisionDebug = true;     // Visual debugging for octree collision (highlights intersecting leaf nodes)
	
	bool bLanderLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

	vector<Box> bboxList;
	vector<ofColor> levelColors;  // Colors for different octree levels

	const float selectionRange = 4.0;
	
	// LEM Physics & Controls
	Lander landerPhysics;
	bool moveForward = false;
	bool moveBack = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool thrustUp = false;
	bool rotateCCW = false;
	bool rotateCW = false;
	bool bPhysicsEnabled = false;  // Toggle physics mode vs drag mode

	// Assets: background image and sounds
	ofImage backgroundImage;
	bool bBackgroundLoaded = false;
	ofSoundPlayer ambientSound;
	bool bAmbientLoaded = false;
	ofSoundPlayer thrustSound;
	bool bThrustLoaded = false;
};
