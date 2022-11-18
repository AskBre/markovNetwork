#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetFrameRate(60);

	box2d.init();
	box2d.setGravity(0,0);
	box2d.createBounds();

	midiMarkov.setup();

}

//--------------------------------------------------------------
void ofApp::update(){
	box2d.update();
	midiMarkov.update();

	if(midiMarkov.newCircles.size()) {
		addCircle(midiMarkov.newCircles.back());
		midiMarkov.newCircles.pop_back();
	}

	if(midiMarkov.newJoints.size()) {
		unsigned int o = midiMarkov.newJoints.back().originCircle;
		unsigned int d = midiMarkov.newJoints.back().destinationCircle;
		addJoint(o, d, 100);
		midiMarkov.newJoints.pop_back();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	for(auto &joint : joints) {
		ofFill();
		ofSetHexColor(0xAAAAAA);
		joint->draw();
	}

	for(int i=0; i<circles.size(); i++) {
		ofFill();
		if(midiMarkov.playIndex == i) {
			ofSetHexColor(0x333333);
		} else {
			ofSetHexColor(0xDDDDDD);
		}
		circles.at(i)->draw();
	}
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

//--------------------------------------------------------------
void ofApp::addCircle(shared_ptr<ofxBox2dCircle> circle) {
	ofVec2f pos;

//	pos.x = ofGetWidth()/2 + ofRandom(-20);
//	pos.y = ofGetHeight()/2 + ofRandom(-20);

	pos.x = ofRandom(ofGetWidth());
	pos.y = ofRandom(ofGetHeight());

	circle->setPhysics(3.0, 0.53, 0.9);
	circle->setup(box2d.getWorld(), pos.x, pos.y, ofRandom(10, 20));
	circle->setMassFromShape = true;
//	circle->addRepulsionForce(pos, 1.2);

	circles.push_back(circle);

	int nCircles = circles.size()-1;
	if(nCircles) {
		addJoint(circles.size()-1, circles.size()-2, 1000);
	}
}

void ofApp::addJoint(unsigned int originIndex, unsigned int destinationIndex, unsigned int jointLength) {
	auto joint = make_shared<ofxBox2dJoint>(box2d.getWorld(), circles.at(originIndex)->body,
			circles.at(destinationIndex)->body);
	ofVec2f dPos = circles.at(destinationIndex)->getPosition();
	ofVec2f oPos = circles.at(originIndex)->getPosition();

	int length = ofDist(dPos.x, dPos.y, oPos.x, oPos.y);	
	joint->setLength(length);
	joints.push_back(joint);
}
