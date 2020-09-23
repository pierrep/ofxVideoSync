#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxVideoSyncBase.h"

#ifdef TARGET_RASPBERRY_PI
    #include "ofxOMXPlayer.h"
#endif

class ofxVideoSyncReceiver : public ofxVideoSyncBase  {
    public:
        ~ofxVideoSyncReceiver();
        void setup();
        void update();

    protected:
        void updateSync();
        void loadSettings();

        ofxOscReceiver oscReceiver;

        float position_sender;
        float position_receiver;
        bool wait_for_sync;
        bool wait_after_sync;
        float wait_after_sync_timer;
        float deviation;
        deque<float> deviations;
        float sync_timer;
        float median_deviation;
        float sync_tolerance;
        
        float seekTime;
        int frameCount;
};
