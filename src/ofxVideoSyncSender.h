#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ofxVideoSyncSender {
    public:
        void setup(ofVideoPlayer* video);
        void update();

    protected:
        ofxOscSender oscSender;
        ofxOscMessage msg;
        
        ofVideoPlayer* video;
        int totalFrames;
        float vidDuration;

};
