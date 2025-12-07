#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	cout << "=== Application starting ===" << endl;
	
	ofSetupOpenGL(1280, 1024,OF_WINDOW);			// <-------- setup the GL context
	cout << "OpenGL context created" << endl;

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	cout << "Starting application..." << endl;
	ofRunApp(new ofApp());
	cout << "Application started" << endl;

}
