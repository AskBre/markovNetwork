#pragma once

#include "ofMain.h"
#include "RtMidi.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void setupMidi();
		void updatePianoMidiIn();
		void updateMarkovMidiIn();

		bool isAutoPopulateVisuals = true;

		int getIndexInNodeNames(unsigned int note);

		void addNode(unsigned int nodeName);
		void addSynapse(ofVec3f origPos, ofVec3f destPos);

		int markovNoteIndex = -1;
		int noteIndex = -1;
		int prevNoteIndex = -1;

		RtMidiIn *markovMidiIn = 0;
		RtMidiIn *pianoMidiIn = 0;

		vector<ofNode> nodes;
		vector<ofPolyline> synapses;
		vector<unsigned int> nodeNames;

		ofCamera cam;
		ofLight light1;
		ofLight light2;
};
