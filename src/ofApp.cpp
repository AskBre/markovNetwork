#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetFrameRate(60);
	ofEnableDepthTest();

	light1.setPosition(-100, 200,100);
	light2.setPosition(100, -200,100);
	cam.lookAt(ofVec3f(0,0,0));

	setupMidi();
}

void ofApp::update(){
	updatePianoMidiIn();
	updateMarkovMidiIn();
}

void ofApp::draw(){
	cam.begin();
	float longitude = (ofGetFrameNum()%2880)*0.125;
	cam.orbitDeg(longitude, longitude, 500);
	ofEnableLighting();
	light1.enable();
	light2.enable();

	ofSetHexColor(0xFFFFFF);
	for(int i=0; i<nodes.size(); i++) {
		ofFill();
		if(markovNoteIndex == i) {
			ofSetHexColor(0xFF3333);
		} else {
			ofSetHexColor(0xDDDDDD);
		}
		ofSpherePrimitive sphere;
		sphere.setPosition(nodes.at(i).getPosition());
		sphere.setRadius(3);
		sphere.drawWireframe();
	}

	ofSetHexColor(0xCCCCCC);
	for(auto synapse : synapses) {
		synapse.draw();
	}

	light1.disable();
	light2.disable();
	cam.end();
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
			noteIndex = getIndexInNodeNames(noteName);
			if(noteIndex == -1) {
				addNode(noteName);
			} 
			if (noteIndex != -1 && prevNoteIndex != -1) {
				ofVec3f o = nodes.at(prevNoteIndex).getPosition();
				ofVec3f d = nodes.at(noteIndex).getPosition();
				addSynapse(o, d);
			}
			prevNoteIndex = noteIndex;
		}
	}
}


void ofApp::updateMarkovMidiIn() {
	vector<unsigned char> midiMessage;
	int nBytes;
	double midiStamp;

	midiStamp = markovMidiIn->getMessage(&midiMessage);

	if(midiMessage.size()) {
		bool isOnNote = midiMessage.at(0) == 144;
		if(isOnNote) {
			markovNoteIndex = getIndexInNodeNames(midiMessage.at(1));
		}
	}
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

	int dist = 100;
	pos.x = ofRandom(-dist, dist);
	pos.y = ofRandom(-dist, dist);
	pos.z = ofRandom(-dist, dist);

	node.setPosition(pos);
	nodes.push_back(node);
	nodeNames.push_back(nodeName);

	int nNodes = nodes.size()-1;
	if(nNodes) {
		ofVec3f pos1 = nodes.at(nodes.size()-1).getPosition();
		ofVec3f pos2 = nodes.at(nodes.size()-2).getPosition();
		addSynapse(pos1, pos2);
	}
}

void ofApp::addSynapse(ofVec3f origPos, ofVec3f destPos) {
	float maxWidth = 10;

	ofPolyline synapse;
	ofVec3f middlePos;
	middlePos = origPos.getInterpolated(destPos, 0.5);

	ofVec3f randPos;
	randPos.x = ofRandom(-maxWidth, maxWidth);
	randPos.y = ofRandom(-maxWidth, maxWidth);
	randPos.z = ofRandom(-maxWidth, maxWidth);
	ofVec3f cPos = middlePos + randPos;

	synapse.addVertex(origPos);
	synapse.quadBezierTo(origPos.x, origPos.y, origPos.z,
			cPos.x, cPos.y, cPos.z,
			destPos.x, destPos.y, destPos.z);

	synapses.push_back(synapse);
}
