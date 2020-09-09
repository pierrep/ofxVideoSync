#include "ofxVideoSyncReceiver.h"

#define SYNC_GRACE_TIME 5
#define SYNC_JUMP_AHEAD 3

ofxVideoSyncReceiver::~ofxVideoSyncReceiver()
{
    if (!xml.save("settings.xml")) {
        ofLogError() << "Couldn't save settings.xml";
    }
}

void ofxVideoSyncReceiver::setup()
{
    totalFrames = video.getTotalNumFrames();
    vidDuration = video.getDuration();
    loadSettings();
    oscReceiver.setup(12345);
}

void ofxVideoSyncReceiver::load(const string name)
{
    #ifdef TARGET_RASPBERRY_PI
    ofxOMXPlayerSettings settings;
    settings.videoPath = name;
    settings.enableTexture = true;
    settings.enableLooping = true;
    settings.enableAudio = false;

    video.setup(settings);

    #else
    video.load(name);
    #endif
}


void ofxVideoSyncReceiver::play()
{
    video.play();
}

void ofxVideoSyncReceiver::draw(float x, float y, float w, float h)
{
    video.draw(x,y,w,h);
}

void ofxVideoSyncReceiver::draw(float x, float y)
{
    video.draw(x,y);
}

void ofxVideoSyncReceiver::updateSync()
{
    if (sync_timer > 0.0f)
        return;

    while (oscReceiver.hasWaitingMessages()) {
        // get the next message
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);

        // check for mouse moved message
        if (msg.getAddress() == "/sync/position") {
            position_master = msg.getArgAsFloat(0);
            position_slave = video.getPosition() * vidDuration;
            deviation = position_master - position_slave;

            ofLogVerbose() << "Deviation: " << deviation;

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
                //kick of the sync process
                sync_timer = ofGetElapsedTimef();
            }
        }
    }
}

void ofxVideoSyncReceiver::update()
{
    updateSync();

    if (wait_for_sync && (sync_timer > 0.0f)) {
        float counter = fabs(deviation) - (ofGetElapsedTimef() - sync_timer);
        //ofLogNotice() << "wait_for_sync counter = " << counter << "  deviation = " << fabs(deviation);
        if (counter < 0.0f) {
            ofLogNotice() << "we are synchronised, play...";
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

    //ofLogNotice() << "Master position: " << position_master << "  Slave position: " << position_slave << " Deviation: " << deviation << " Median deviation: " << median_deviation;

    if ((fabs(median_deviation) > sync_tolerance) && (position_slave > SYNC_GRACE_TIME) && (position_master > SYNC_GRACE_TIME) && (!wait_for_sync)) {
        ofLogNotice() << "Time: " << ofGetTimestampString("%H:%M %S secs") << "  Master position: " << position_master << " Slave position: " << position_slave << " Deviation: " << deviation << " Median deviation: " << median_deviation;
        if (position_master < vidDuration) {
#ifdef TARGET_RASPBERRY_PI
            video.seekToTimeInSeconds((position_master + SYNC_JUMP_AHEAD) / vidDuration);
#else
            video.setPosition((position_master + SYNC_JUMP_AHEAD) / vidDuration);
            ofLogNotice() << "Jump ahead to: " << (position_master + SYNC_JUMP_AHEAD) / vidDuration;
#endif


        }
        video.update();
        video.setPaused(true);

        wait_for_sync = true;
        wait_after_sync = ofGetElapsedTimef();
    }

    video.update();
}

void ofxVideoSyncReceiver::loadSettings()
{
    if (!xml.load("settings.xml")) {
        ofLogWarning() << "Couldn't load settings file. We will create it instead.";
    }
    auto settings = xml.getChild("settings");
    if (!settings) {
        settings = xml.appendChild("settings");
    }

    ofXml entry = settings.findFirst("sync_tolerance");
    if (!entry) {
        // remove any possible malformed tag
        settings.removeChild("sync_tolerance");
        settings.appendChild("sync_tolerance").set(0.05f);
    }

    auto tolerance = xml.findFirst("//sync_tolerance");
    if (tolerance) {
        sync_tolerance = tolerance.getFloatValue();
        ofLogNotice() << "Set sync tolerance to: " << sync_tolerance;
    }
}
