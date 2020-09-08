#include "ofxVideoSyncSender.h"

void ofxVideoSyncSender::setup(ofVideoPlayer* _video, bool _localhost)
{
    bUseLocalhost = _localhost;
    video = _video;
    totalFrames = video->getTotalNumFrames();
    vidDuration = video->getDuration();

    if (bUseLocalhost) {
        ofLogNotice() << "broadcasting on localhost";
        oscSender.setup("localhost", 12345);
    } else {
        string ip = getBroadcastIP();
        ofLogNotice() << "broadcasting on " << ip;
        oscSender.setup(ip, 12345);
    }
}

void ofxVideoSyncSender::update()
{
    float currentPosition = video->getPosition() * vidDuration;

    msg.clear();
    msg.setAddress("/sync/position");
    msg.addFloatArg(currentPosition);
    oscSender.sendMessage(msg, false);
}

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
