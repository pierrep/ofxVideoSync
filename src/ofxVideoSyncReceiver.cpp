#include "ofxVideoSyncReceiver.h"

#define SYNC_GRACE_TIME 5
#define SYNC_JUMP_AHEAD 3

ofxVideoSyncReceiver::~ofxVideoSyncReceiver()
{
    ofLogNotice() << "ofxVideoSyncReceiver destructed";

}

//---------------------------------------------------------------------------
void ofxVideoSyncReceiver::setup()
{
    ofxVideoSyncBase::setup();    
    ofLogVerbose("ofxVideoSyncReceiver") << "setup()";

    loadSettings();
    oscReceiver.setup(12345);
}

//---------------------------------------------------------------------------
void ofxVideoSyncReceiver::updateSync()
{
    if (sync_timer > 0.0f)
        return;

    while (oscReceiver.hasWaitingMessages()) {
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);
        
        if (msg.getAddress() == "/sync/loop") {
            #ifdef TARGET_RASPBERRY_PI
            video.restartMovie();
            #else
            ofLogNotice() << "Received /sync/loop at " << ofGetTimestampString("%H:%M:%S secs");
            video.setPaused(true);
            video.setPosition(0);
            video.play();
            #endif
            
            wait_for_sync = false;
            wait_after_sync = false;
            deviations.clear();
            wait_after_sync_timer = 0.0f;
        }
        else if (msg.getAddress() == "/sync/position") {
            position_sender = msg.getArgAsFloat(0);

            #ifdef TARGET_RASPBERRY_PI
            position_receiver = video.getMediaTime();
            #else
            position_receiver = video.getPosition() * vidDuration;
            #endif
            deviation = position_sender - position_receiver;

            deviations.push_back(deviation);

            if (deviations.size() > 10) {
                deviations.pop_front();
            }

            deque<float> sorted = deviations;
            sort(sorted.begin(), sorted.end());

            median_deviation = 0;
            if (sorted.size() == 10) {
                median_deviation = sorted.at(5);
            }

            if (wait_for_sync) {                            
            #ifdef TARGET_RASPBERRY_PI 
                video.setPaused(true);
                frameCount++;
                if(frameCount > 5) {
                   sync_timer = ofGetElapsedTimef(); 
                   frameCount = 0;
                }     
            #else
                sync_timer = ofGetElapsedTimef(); 
            #endif
                ofLogNotice() << "Kick off sync process... Sender: " << position_sender << " Receiver: " << position_receiver << " Deviation: " << deviation;

            }
        }
    }
}

//---------------------------------------------------------------------------
void ofxVideoSyncReceiver::update()
{
    updateSync();

    if (wait_for_sync && (sync_timer > 0.0f)) {
        float counter = fabs(deviation) - (ofGetElapsedTimef() - sync_timer);

        #ifdef TARGET_RASPBERRY_PI
        if (counter < 0.003f) {
            position_receiver = video.getMediaTime();
        #else
        if (counter < 0.0f) {            
            position_receiver = video.getPosition() * vidDuration;
        #endif
            ofLogNotice() << "we are synchronised, play..." << "Receiver: " << position_receiver << " Sender: " << position_sender << " Counter: " << counter;
            video.setPaused(false);

            wait_for_sync = false;
            wait_after_sync = true;
            wait_after_sync_timer = ofGetElapsedTimef();
            deviations.clear();
            sync_timer = 0.0f;
        }
        return;
    }

    if (wait_after_sync) {
        if ((ofGetElapsedTimef() - wait_after_sync_timer) > SYNC_GRACE_TIME) {
            wait_after_sync = false;
            ofLogVerbose() << "Wait after sync expired...";
        }
        return;
    }

    //if ((fabs(median_deviation) > sync_tolerance) && (position_receiver > SYNC_GRACE_TIME) && (position_sender > SYNC_GRACE_TIME) && (!wait_for_sync)) {
    if ((fabs(median_deviation) > sync_tolerance) && (!wait_for_sync)) {
        ofLogNotice() << "*** Out of Sync! Time: " << ofGetTimestampString("%H:%M:%S secs") << "  Sender: " << position_sender << " Receiver: " << position_receiver << " Deviation: " << deviation << " Median deviation: " << median_deviation;
        if (position_sender < (vidDuration - SYNC_JUMP_AHEAD)) {
    #ifdef TARGET_RASPBERRY_PI
            int seekTimeNew = position_sender + SYNC_JUMP_AHEAD;
            seekTime = seekTimeNew;
            ofLogNotice("ofxVideoSyncReceiver") << "Seek time: " << seekTime;
            video.seekToTimeInSeconds(seekTime);
    #else
            video.setPosition((position_sender + SYNC_JUMP_AHEAD) / vidDuration);
            ofLogNotice() << "Jump ahead to: " << (position_sender + SYNC_JUMP_AHEAD) / vidDuration;
    #endif
        }
        
    #ifdef TARGET_RASPBERRY_PI
        video.setPaused(false);
        
    #else
        video.update();
        video.setPaused(true);
    #endif

        wait_for_sync = true;
        wait_after_sync = ofGetElapsedTimef();
    }

    #ifndef TARGET_RASPBERRY_PI
    video.update();
    #endif
}

//---------------------------------------------------------------------------
void ofxVideoSyncReceiver::loadSettings()
{
    if (!xml.load("sync_settings.xml")) {
        ofLogWarning() << "Couldn't load settings file. We will create it instead.";
    }
    auto settings = xml.getChild("settings");
    if (!settings) {
        settings = xml.appendChild("settings");
    }

    ofXml entry2 = settings.findFirst("sync_tolerance");
    if (!entry2) {
        // remove any possible malformed tag
        settings.removeChild("sync_tolerance");
        settings.appendChild("sync_tolerance").set(0.05f);
    }

    auto tolerance = xml.findFirst("//sync_tolerance");
    if (tolerance) {
        sync_tolerance = tolerance.getFloatValue();
        ofLogNotice() << "Set sync tolerance to: " << sync_tolerance;
    }

    if (!xml.save("sync_settings.xml")) {
        ofLogError() << "Couldn't save settings.xml";
    }
}
