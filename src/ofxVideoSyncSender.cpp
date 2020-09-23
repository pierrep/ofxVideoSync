#include "ofxVideoSyncSender.h"

#ifdef TARGET_RASPBERRY_PI
void ofxVideoSyncSender::onVideoLoop(ofxOMXPlayer* player)
{
    msg.clear();
    msg.setAddress("/sync/loop");
    msg.addIntArg(1);
    oscSender.sendMessage(msg, false);
    ofLogNotice() << "*** onVideoLoop called";
}
#endif

//---------------------------------------------------------------------------
void ofxVideoSyncSender::setup()
{
    ofxVideoSyncBase::setup();
    ofLogVerbose("ofxVideoSyncReceiver") << "setup()";

    //ignore localhost for the moment
    bUseLocalhost = false;

    if (bUseLocalhost) {
        ofLogNotice("ofxVideoSyncSender") << "broadcasting on localhost";
        oscSender.setup("localhost", 12345);
    } else {
        string ip = getBroadcastIP();
        ofLogNotice("ofxVideoSyncSender") << "broadcasting on " << ip;
        oscSender.setup(ip, 12345);
    }
}

//---------------------------------------------------------------------------
void ofxVideoSyncSender::update()
{

    #ifndef TARGET_RASPBERRY_PI
    if(video.getIsMovieDone()) {
        ofLogVerbose()<< "Movie is done ...looping";
        msg.clear();
        msg.setAddress("/sync/loop");
        msg.addIntArg(1);
        oscSender.sendMessage(msg, false);
    }
    video.update();
    #endif

    float currentPosition;
    #ifdef TARGET_RASPBERRY_PI
    currentPosition = video.getMediaTime();
    #else
    currentPosition = video.getPosition() * vidDuration;
    #endif

    msg.clear();
    msg.setAddress("/sync/position");
    msg.addFloatArg(currentPosition);
    oscSender.sendMessage(msg, false);
}

//---------------------------------------------------------------------------
string ofxVideoSyncSender::getBroadcastIP()
{
    string s = ofSystem("ifconfig");
    size_t pos_begin = s.find("broadcast ");
    string sub = s.substr(pos_begin + 10);
    string broadcast_ip;

    for (int i = 0; i < 3; i++) {
        size_t pos = sub.find(".");
        broadcast_ip += sub.substr(0, pos) + ".";
        sub = sub.substr(pos + 1);
    }
    size_t pos_end = sub.find("\n");
    broadcast_ip += sub.substr(0, pos_end);

    return broadcast_ip;
}
