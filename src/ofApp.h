#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
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

		int getIndexInCircleNotes(unsigned int note);

		void addCircle(unsigned int circleName);
		void addJoint(unsigned int originIndex, unsigned int destinationIndex);

		int markovNoteIndex = -1;
		int prevMarkovNoteIndex = -1;

		RtMidiIn *markovMidiIn = 0;
		RtMidiIn *pianoMidiIn = 0;

		ofxBox2d box2d;
		vector<unsigned int> circleNames;
		vector<shared_ptr<ofxBox2dCircle>> circles;

		vector<shared_ptr<ofxBox2dJoint>> joints;

};
