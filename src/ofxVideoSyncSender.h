#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#ifdef TARGET_RASPBERRY_PI
    #include "ofxOMXPlayer.h"
#endif

class ofxVideoSyncSender {
    public:
#ifdef TARGET_RASPBERRY_PI
        ofxOMXPlayer load(ofxOMXPlayerSettings settings);
#else
    void load(const string video);
#endif
        void setup(bool _localhost = false);
        void play();
        void draw(float x, float y, float w, float h);
        void draw(float x, float y);
        void update();

    protected:
        string getBroadcastIP();

        ofxOscSender oscSender;
        ofxOscMessage msg;
        
        int totalFrames;
        float vidDuration;
        bool bUseLocalhost;

#ifdef TARGET_RASPBERRY_PI
        ofxOMXPlayer video;
#else
        ofVideoPlayer video;
#endif
};
