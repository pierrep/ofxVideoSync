#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#ifdef TARGET_RASPBERRY_PI
    #include "ofxOMXPlayer.h"
#endif

class ofxVideoSyncReceiver {
    public:
        ~ofxVideoSyncReceiver();
        void setup();
        void load(const string video);
        void play();
        void draw(float x, float y, float w, float h);
        void draw(float x, float y);
        void updateSync();
        void update();

    protected:
        void loadSettings();

        ofxOscReceiver oscReceiver;

#ifdef TARGET_RASPBERRY_PI
        ofxOMXPlayer video;
#else
        ofVideoPlayer video;
#endif

        int totalFrames;
        float vidDuration;
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
        ofXml xml;
        
        float seekTime;
        int frameCount;
};
