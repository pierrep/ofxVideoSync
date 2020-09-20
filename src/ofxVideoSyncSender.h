#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#ifdef TARGET_RASPBERRY_PI
    #include "ofxOMXPlayer.h"
#endif

class ofxVideoSyncSender : public ofxOMXPlayerListener {
    public:

        ~ofxVideoSyncSender();
        void load(const string video);
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
        void onVideoLoop(ofxOMXPlayer* player);
        void onVideoEnd(ofxOMXPlayer* player) {};
#else
        ofVideoPlayer video;
#endif
};
