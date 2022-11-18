#pragma once

#include "RtMidi.h"

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <algorithm>
#include "ofxBox2d.h"

using namespace std;

struct MarkovMessage_t {
	vector<unsigned char> message;
	double stamp = 0;
	// Its own position in Markovmessages
	int pos;
	// Vector of messages succeeding this one (position in MarkovMessages)
	vector<unsigned int> succeedingMessages;

};

struct NewJoint_t {
	unsigned int originCircle;
	unsigned int destinationCircle;
};

class MidiMarkov {
	public:
		int setup();
		int cleanup();
		void update();

		int playIndex = 0;

		vector<MarkovMessage_t> markovMessages;
		vector<shared_ptr<ofxBox2dCircle>> newCircles;
		vector<NewJoint_t> newJoints;


	private:
		RtMidiIn *midiin = 0;
		RtMidiOut *midiout = 0;

		MarkovMessage_t curMarkovMessage;
		MarkovMessage_t prevMarkovMessage;
		int prevPosition = -1;
		double curStamp;
		chrono::time_point<chrono::high_resolution_clock> prevTime;

		void updateMarkovMessages();
		void iterateMarkovChain();
		int getPositionInMarkovMessages(MarkovMessage_t &markovMessage);

		void printMarkovMessage(MarkovMessage_t &markovMessage);
		void printMarkovMessages();

		void addCircle();	


};
