#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetFrameRate(60);

	setupMidi();
}

void ofApp::update(){
	updatePianoMidiIn();
//	updateMarkovMidiIn();
}

void ofApp::draw(){
	cam.begin();
	ofEnableLighting();
	light1.enable();
	/*
	for(auto &joint : joints) {
		ofFill();
		ofSetHexColor(0xAAAAAA);
		joint->draw();
	}
	*/

	ofSetHexColor(0xFFFFFF);
	for(int i=0; i<nodes.size(); i++) {
		ofFill();
		if(markovNoteIndex == i) {
			ofSetHexColor(0xFF3333);
		} else {
			ofSetHexColor(0xDDDDDD);
		}
		ofVec3f pos = nodes.at(i).getPosition();
		ofDrawSphere(pos, 10);
	}

	ofLine(0, 0, ofGetWidth(), ofGetHeight());
	light1.disable();
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y){

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){

}

void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::mouseEntered(int x, int y){

}

void ofApp::mouseExited(int x, int y){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit() {
	delete markovMidiIn;
	delete pianoMidiIn;
}

//--------------------------------------------------------------
void ofApp::setupMidi() {
	try {
		markovMidiIn = new RtMidiIn();
		pianoMidiIn = new RtMidiIn();
	} catch (RtMidiError &error) {
		error.printMessage();
	}

	markovMidiIn->openVirtualPort("MarkovIn");
	markovMidiIn->ignoreTypes(false, false, false);

	pianoMidiIn->openVirtualPort("PianoIn");
	pianoMidiIn->ignoreTypes(false, false, false);

}

void ofApp::updatePianoMidiIn() {
	vector<unsigned char> midiMessage;
	int nBytes;
	double midiStamp;

	midiStamp = pianoMidiIn->getMessage(&midiMessage);

	if(midiMessage.size()) {
		bool isOnNote = midiMessage.at(0) == 144;
		if(isOnNote) {
			int noteName = midiMessage.at(1);
			int i = getIndexInNodeNames(noteName);
			if(i == -1) {
				addNode(noteName);
			}
		}
	}
}

void ofApp::updateMarkovMidiIn() {
	/*
	vector<unsigned char> midiMessage;
	int nBytes;
	double midiStamp;

	midiStamp = markovMidiIn->getMessage(&midiMessage);

	if(midiMessage.size()) {
		bool isOnNote = midiMessage.at(0) == 144;
		if(isOnNote) {
			markovNoteIndex = getIndexInCircleNotes(midiMessage.at(1));
			if(markovNoteIndex != -1 && prevMarkovNoteIndex != -1) {
				unsigned int o = circleNames.at(prevMarkovNoteIndex);
				unsigned int d = circleNames.at(markovNoteIndex);
				addJoint(o, d);
			} else if(markovNoteIndex == -1) {
				cout << "Can't find " << midiMessage.at(1) << " in circles.";
			}

			prevMarkovNoteIndex = markovNoteIndex;
		}
	}
	*/
}

int ofApp::getIndexInNodeNames(unsigned int note) {
	for(unsigned int i = 0; i < nodeNames.size(); i++) {
		if(nodeNames.at(i) == note) {
			return i;
		}
	}

	return -1;
}

void ofApp::addNode(unsigned int nodeName) {
	ofNode node;
	ofVec3f pos;

	pos.x = ofRandom(0-ofGetWidth());
	pos.y = ofRandom(ofGetHeight());
	pos.z = ofRandom(0-ofGetWidth());
	pos.z -= 100;

	node.setPosition(pos);
	nodes.push_back(node);
	nodeNames.push_back(nodeName);

	/*
	int nCircles = circles.size()-1;
	if(nCircles) {
		addJoint(circles.size()-1, circles.size()-2);
	}
	*/
}

void ofApp::addJoint(unsigned int originIndex, unsigned int destinationIndex) {
	/*
	if(originIndex >= circles.size() || destinationIndex >= circles.size()) {
		cout << "Out of Bounds in joint making" << endl;
		return;
	}

	auto joint = make_shared<ofxBox2dJoint>(box2d.getWorld(), circles.at(originIndex)->body,
			circles.at(destinationIndex)->body);
	ofVec2f dPos;
	ofVec2f oPos;

	if(destinationIndex < circles.size()){
		dPos = circles.at(destinationIndex)->getPosition();
	} else {
		cout << "destinationIndex " << destinationIndex << " out of bounds" << endl;
		return;
	}

	if(originIndex < circles.size()){
		oPos = circles.at(originIndex)->getPosition();
	} else {
		cout << "originIndex " << originIndex << " out of bounds" << endl;
		return;
	}

	int length = ofDist(dPos.x, dPos.y, oPos.x, oPos.y);	
	joint->setLength(length);
	joints.push_back(joint);
	*/
}
