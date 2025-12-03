#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include "SpaceLander.h"
#include <glm/gtx/intersect.hpp>



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

		ofEasyCam cam;
		ofxAssimpModelLoader mars, lander;
		ofLight light;
		Box boundingBox, landerBounds;
		Box testBox;
		vector<Box> colBoxList;
		bool bLanderSelected = false;
		Octree octree;
		TreeNode selectedNode;
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;


		ofxIntSlider numLevels;
		ofxToggle bTimingInfo;
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
		
		bool bLanderLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		vector<Box> bboxList;

	const float selectionRange = 4.0;

	bool resolvingCollision = false;

	// Free camera movement variables
	bool bArrowUp = false;
	bool bArrowDown = false;
	bool bArrowLeft = false;
	bool bArrowRight = false;
	float cameraMoveSpeed = 0.5f;

	// SpaceLander physics simulation
	SpaceLander spaceLander;
	float lastFrameTime;  // For calculating deltaTime
	
	// Lander control inputs
	bool bThrustKey = false;      // Space bar for main thrust
	bool bForwardKey = false;     // W key
	bool bBackwardKey = false;    // S key
	bool bLeftKey = false;        // A key
	bool bRightKey = false;       // D key
	bool bUpKey = false;          // R key
	bool bDownKey = false;        // F key
	bool bYawLeftKey = false;     // Q key
	bool bYawRightKey = false;    // E key
	bool bPitchUpKey = false;     // I key
	bool bPitchDownKey = false;   // K key
	bool bRollLeftKey = false;    // J key
	bool bRollRightKey = false;   // L key
};
