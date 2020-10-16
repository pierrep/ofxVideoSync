#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxVideoSyncBase.h"

#ifdef TARGET_RASPBERRY_PI
    #include "ofxOMXPlayer.h"
#endif

#ifdef TARGET_RASPBERRY_PI
class ofxVideoSyncSender : public ofxVideoSyncBase, public ofxOMXPlayerListener {
#else
class ofxVideoSyncSender : public ofxVideoSyncBase {
#endif
    public:
        void load(const string name);
        void setup();
        void update();
        void enableSync();
        void disableSync();

    protected:
        string getBroadcastIP();

        ofxOscSender oscSender;
        ofxOscMessage msg;
        
        bool bUseLocalhost;

#ifdef TARGET_RASPBERRY_PI
        void onVideoLoop(ofxOMXPlayer* player);
        void onVideoEnd(ofxOMXPlayer* player) {};
#endif
};
