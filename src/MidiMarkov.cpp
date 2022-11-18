#include "MidiMarkov.h"

int MidiMarkov::setup() {
	try {
		midiin = new RtMidiIn();
	} catch (RtMidiError &error) {
		error.printMessage();
	}

	midiin->openVirtualPort();
	midiin->ignoreTypes(false, false, false);

	try {
		midiout = new RtMidiOut();
	} catch (RtMidiError &error) {
		error.printMessage();
	}

	midiout->openVirtualPort();

	return 0;
}

int MidiMarkov::cleanup() {
	delete midiin;
	delete midiout;

	return 0;
}

void MidiMarkov::update() {
	updateMarkovMessages();
	iterateMarkovChain();
}

void MidiMarkov::updateMarkovMessages() {
	//TODO curMarkovMessage does not need to be global
	curMarkovMessage.stamp = midiin->getMessage(&curMarkovMessage.message);

	// Only save if note is shorter than four seconds
	if(curMarkovMessage.stamp < 4 &&curMarkovMessage.message.size() > 0) {
		unsigned int curPosition = getPositionInMarkovMessages(curMarkovMessage);

		if(prevPosition != -1) {
			markovMessages.at(prevPosition).succeedingMessages.push_back(curPosition);
			NewJoint_t newJoint;
			newJoint.originCircle = prevPosition;
			newJoint.destinationCircle = curPosition;
			newJoints.push_back(newJoint);
		}

//		printMarkovMessages();
		prevPosition = curPosition;
	}

}

void MidiMarkov::iterateMarkovChain() {
	if(markovMessages.size() > 1) {
		chrono::time_point<chrono::high_resolution_clock> curTime;
		curTime = chrono::system_clock::now();

		chrono::duration<double> deltaPrevMsg = curTime - prevTime;
		chrono::duration<double> stampDuration = (chrono::duration<double>) markovMessages.at(playIndex).stamp;

		if(deltaPrevMsg > stampDuration) {
			unsigned int maxRand = markovMessages.at(playIndex).succeedingMessages.size();
			if(maxRand) { 
				playIndex = markovMessages.at(playIndex).succeedingMessages.at(rand() % maxRand);
				midiout->sendMessage(&markovMessages.at(playIndex).message);
				printMarkovMessage(markovMessages.at(playIndex));
			}

			prevTime = curTime;
		}
	}
}

int MidiMarkov::getPositionInMarkovMessages(MarkovMessage_t &markovMessage) {
	for (int i=0; i<markovMessages.size();i++) {
		if (markovMessages.at(i).message == markovMessage.message){
			return i;
		}
	}

	markovMessage.pos = markovMessages.size();
	markovMessages.push_back(markovMessage);
	addCircle();

	return markovMessage.pos;
}

void MidiMarkov::printMarkovMessage(MarkovMessage_t &markovMessage) {
	int nBytes = markovMessage.message.size();
	cout << markovMessage.pos;

	for(int i=0; i<nBytes; i++) {
		cout << ": Byte " << i << " = " << (int)markovMessage.message[i] << ", ";
	}
	if(nBytes > 0) {
		cout << "stamp = " << markovMessage.stamp << endl; 
	}


	cout << "Succ: ";
	for(auto m : markovMessage.succeedingMessages) {
		cout << m << "|";
	}
	cout << endl;

}

void MidiMarkov::printMarkovMessages() {
	for(int i=0;i<markovMessages.size();i++) {
		if(markovMessages.at(i).succeedingMessages.size()) {
			printMarkovMessage(markovMessages.at(i));
		}
	}
}

void MidiMarkov::addCircle() {

	auto circle = make_shared<ofxBox2dCircle>();
	newCircles.push_back(circle);
}
