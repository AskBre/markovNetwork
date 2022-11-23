#pragma once

#include "ofMain.h"
#include "RtMidi.h"

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
		void exit();

		void setupMidi();
		void updatePianoMidiIn();
		void updateMarkovMidiIn();

		int getIndexInNodeNames(unsigned int note);

		void addNode(unsigned int nodeName);
		void addJoint(unsigned int originIndex, unsigned int destinationIndex);

		int markovNoteIndex = -1;
		int prevMarkovNoteIndex = -1;

		RtMidiIn *markovMidiIn = 0;
		RtMidiIn *pianoMidiIn = 0;

		vector<ofNode> nodes;
		vector<unsigned int> nodeNames;

		ofCamera cam;
		ofLight light1;

//		vector<shared_ptr<ofxBox2dJoint>> joints;

};
