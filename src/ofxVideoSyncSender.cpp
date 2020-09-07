#include "ofxVideoSyncSender.h"


void ofxVideoSyncSender::setup(ofVideoPlayer* _video)
{

    video = _video;
    totalFrames = video->getTotalNumFrames();
    vidDuration = video->getDuration();

    oscSender.setup("localhost", 12345);
}



void ofxVideoSyncSender::update()
{
    float currentPosition = video->getPosition()*vidDuration;

    msg.clear();
    msg.setAddress("/sync/position");
    msg.addFloatArg(currentPosition);
    oscSender.sendMessage(msg, false);  
}
