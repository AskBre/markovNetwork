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
//	updateMarkovMidiIn();
}

void ofApp::draw(){
	cam.begin();
	float longitude = (ofGetFrameNum()%1080) * 0.25;
//	float longitude = 300;
	cam.orbitDeg(0, longitude, 500);
	ofEnableLighting();
	light1.enable();
	light2.enable();
	/*
	for(auto &joint : joints) {
		ofFill();
		ofSetHexColor(0xAAAAAA);
		joint->draw();
	}
	*/

	ofSetHexColor(0xFFFFFF);
	ofDrawAxis(200);
	for(int i=0; i<nodes.size(); i++) {
		ofFill();
		if(markovNoteIndex == i) {
			ofSetHexColor(0xFF3333);
		} else {
			ofSetHexColor(0xDDDDDD);
		}
		ofSpherePrimitive sphere;
		sphere.setPosition(nodes.at(i).getPosition());
		sphere.setRadius(5);
		sphere.drawWireframe();
	}
	for(auto r : ribbons) {
//		r.drawWireframe();
		r.draw();
	}

	for(auto lines : linesLines) {
		for(auto l : lines) {
			l.draw();
		}
	}

	light1.disable();
	light2.disable();
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
		addRibbon(pos1, pos2, 5);
	}
}


void ofApp::addRibbon(ofVec3f origPos, ofVec3f destPos, float maxWidth) {
	unsigned int resolution = 50;
	ofVec3f pos = origPos;
	vector<ofPolyline> lines;

	for(unsigned int i=0; i<resolution; i++) {
		ofPolyline line;
		ofVec3f middlePos;
		middlePos = origPos.getInterpolated(destPos, 0.5);
		float p = ofNormalize(i, 0, resolution);

		ofVec3f randPos;
		randPos.x = ofRandom(-maxWidth, maxWidth);
		randPos.y = ofRandom(-maxWidth, maxWidth);
		randPos.z = ofRandom(-maxWidth, maxWidth);
		ofVec3f cPos = middlePos + randPos;

		line.addVertex(origPos);
		line.quadBezierTo(origPos.x, origPos.y, origPos.z,
				cPos.x, cPos.y, cPos.z,
				destPos.x, destPos.y, destPos.z);
		lines.push_back(line);
	}

	linesLines.push_back(lines);

	/*
	ofMesh ribbon;
//	ribbon.setupIndicesAuto();
	for(int j=0; j<lines.size()-1; j++) {
		for(int i=0; i<lines.at(0).getVertices().size(); i++) {
			ribbon.addVertex(lines.at(j)[i]);
			int nextRib = (j+1)%lines.size();
			ribbon.addVertex(lines.at(nextRib)[i]);
		}
		for(int i=lines.at(0).getVertices().size(); i>0; i--) {
			ribbon.addVertex(lines.at(j)[i]);
			int nextRib = (j-1)%lines.size();
			ribbon.addVertex(lines.at(nextRib)[i]);
		}
	}

	ribbons.push_back(ribbon);
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
