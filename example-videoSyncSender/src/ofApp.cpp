#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    video.load("../../../videos/wifisync.mp4");
    syncSender.setup(&video);
    video.play();

}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    syncSender.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    video.draw(0,0,720,360);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    float value = (float) x / ofGetWidth();
    int frame = video.getTotalNumFrames()*value;
    //ofLogNotice() << "seek to frame: " << frame;
    video.setFrame(frame);
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
