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
	MarkovMessage_t curMarkovMessage;
	curMarkovMessage.stamp = midiin->getMessage(&curMarkovMessage.message);

	bool isMessage = curMarkovMessage.message.size() > 0;
	bool isShort = curMarkovMessage.stamp < 4;

	// Only save if note is shorter than four seconds
	if(isMessage && isShort) {
		unsigned int curPosition = getPositionInMarkovMessages(curMarkovMessage);
		if(curPosition == -1) {
			cout << "Couldn't get position" << endl;
			return;
		}

		if(prevPosition != -1) markovMessages.at(prevPosition).succeedingMessages.push_back(curPosition);

		if(prevPosition > 1) {

			bool isNoteOn = curMarkovMessage.message.at(0) == 144;
//			bool isPrevNoteOn = markovMessages.at(prevPosition-1).message.at(0) == 144;
			if(isNoteOn) {
				NewJoint_t newJoint;
				newJoint.originCircle = floor(prevOnPosition * 0.5);
				newJoint.destinationCircle = floor(curPosition * 0.5);
				newJoints.push_back(newJoint);
				prevOnPosition = curPosition;
			}
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
			playNextNote();
			prevTime = curTime;
		}
	}
}

void MidiMarkov::playNextNote() {
	unsigned int maxRand = markovMessages.at(playIndex).succeedingMessages.size();
	if(maxRand) { 
		playIndex = markovMessages.at(playIndex).succeedingMessages.at(rand() % maxRand);
		midiout->sendMessage(&markovMessages.at(playIndex).message);
		if(markovMessages.at(playIndex).message.at(0) == 144) noteOnIndex = floor(playIndex*0.5);
		printMarkovMessage(markovMessages.at(playIndex));
	}
}

int MidiMarkov::getPositionInMarkovMessages(MarkovMessage_t &markovMessage) {
	bool isNew = true;
	int pos = 1;

	bool isNoteOn = markovMessage.message.at(0) == 144;
	bool isNoteOff = markovMessage.message.at(0) == 128;
	bool isNote = isNoteOn || isNoteOff;

	if(!isNote) {
//		cout << "This is not a note, but that's okay." << endl;
		return -1;
	}

	for (int i=0; i<markovMessages.size();i++) {

		bool isFirstNew = markovMessages.at(i).message.at(0) == markovMessage.message.at(0);
		bool isSecondNew = markovMessages.at(i).message.at(1) == markovMessage.message.at(1);

		if (isFirstNew && isSecondNew){
//			cout << "Is Not New" << endl;
			isNew = false;
			pos = i;
		}
	}

	if(isNew && isNote) {
		markovMessage.pos = markovMessages.size();
		markovMessages.push_back(markovMessage);
//		cout << "Is New" << endl;
//		printMarkovMessage(markovMessage);
		if(isNoteOn) addCircle();
		pos = markovMessage.pos;
	}

	return pos;
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
