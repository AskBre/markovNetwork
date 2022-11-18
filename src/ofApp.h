#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "MidiMarkov.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		MidiMarkov midiMarkov;
		unsigned prevMMarkovSize();

		ofxBox2d box2d;
		vector<shared_ptr<ofxBox2dCircle>> circles;
		vector<shared_ptr<ofxBox2dJoint>> joints;

		void addCircle(shared_ptr<ofxBox2dCircle> circle);
		void addJoint(unsigned int originIndex, unsigned int destinationIndex, unsigned int jointLength);
};
