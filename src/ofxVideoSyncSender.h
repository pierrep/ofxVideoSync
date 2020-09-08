#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ofxVideoSyncSender {
    public:
        void setup(ofVideoPlayer* video, bool _localhost = false);
        void update();

    protected:
        string getBroadcastIP();

        ofxOscSender oscSender;
        ofxOscMessage msg;
        
        ofVideoPlayer* video;
        int totalFrames;
        float vidDuration;
        bool bUseLocalhost;
};
