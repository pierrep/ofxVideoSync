#include "ofxVideoSyncReceiver.h"

#define SYNC_TOLERANCE .05
#define SYNC_GRACE_TIME 5
#define SYNC_JUMP_AHEAD 3

void ofxVideoSyncReceiver::setup(ofVideoPlayer* _video)
{

    slaveVideo = _video;
    totalFrames = slaveVideo->getTotalNumFrames();
    vidDuration = slaveVideo->getDuration();

    oscReceiver.setup(12345);
}

void ofxVideoSyncReceiver::updateSync()
{
    if(sync_timer > 0.0f) return;

    while(oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);

        // check for mouse moved message
        if(msg.getAddress() == "/sync/position")
        {
            position_master = msg.getArgAsFloat(0);
            position_slave = slaveVideo->getPosition() * vidDuration;
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

            if(wait_for_sync) {
                //kick of the sync process
                sync_timer = ofGetElapsedTimef();
            }
        }
    }


}

void ofxVideoSyncReceiver::update()
{
    updateSync();

    if(wait_for_sync && (sync_timer > 0.0f)) {
            float counter = fabs(deviation) - (ofGetElapsedTimef() - sync_timer);
            //ofLogNotice() << "wait_for_sync counter = " << counter << "  deviation = " << fabs(deviation);
            if (counter < 0.0f)
            {
                ofLogNotice() << "we are synchronised, play...";
                slaveVideo->setPaused(false);

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

    if ((fabs(median_deviation) > SYNC_TOLERANCE) && (position_slave > SYNC_GRACE_TIME) && (position_master > SYNC_GRACE_TIME) &&(!wait_for_sync))
    {
        ofLogNotice() << "Time: " << ofGetTimestampString("%H:%M %S secs") << "  Master position: " << position_master << " Slave position: " << position_slave << " Deviation: " << deviation << " Median deviation: " << median_deviation;
        if (position_master < vidDuration) {
            slaveVideo->setPosition((position_master + SYNC_JUMP_AHEAD)/vidDuration);
            ofLogNotice() << "Jump ahead to: " << (position_master + SYNC_JUMP_AHEAD)/vidDuration;
        }
        slaveVideo->update();
        slaveVideo->setPaused(true);

        wait_for_sync = true;
        wait_after_sync = ofGetElapsedTimef();
    }
}
