#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ofxVideoSyncReceiver {
    public:
        ~ofxVideoSyncReceiver();
        void setup();

#ifdef TARGET_RASPBERRY_PI
        ofxOMXPlayer setup(ofxOMXPlayerSettings settings);
#endif
        void load(const string video);
        void play();
        void draw(float x, float y, float w, float h);
        void draw(float x, float y);
        void updateSync();
        void update();

    protected:
        void loadSettings();

        ofxOscReceiver oscReceiver;
        ofVideoPlayer video;
        int totalFrames;
        float vidDuration;
        float position_master;
        float position_slave;
        bool wait_for_sync;
        bool wait_after_sync;
        float wait_after_sync_timer;
        float deviation;
        deque<float> deviations;
        float sync_timer;
        float median_deviation;
        float sync_tolerance;
        ofXml xml;
};
