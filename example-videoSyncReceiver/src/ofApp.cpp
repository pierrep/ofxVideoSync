#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){    
    ofSetVerticalSync(false);
    ofSetFrameRate(30);

    //syncReceiver.setType(SYNC_RECEIVER);
    syncReceiver.load("../../../videos/wifisync.mp4");
    syncReceiver.play();

#ifndef TARGET_RASPBERRY_PI
    ofSetWindowTitle("Receiver");
#endif
}

//--------------------------------------------------------------
void ofApp::update(){

    syncReceiver.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    syncReceiver.draw(0,0,ofGetWidth(),ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
