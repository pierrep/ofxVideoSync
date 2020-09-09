#include "ofxVideoSyncSender.h"


void ofxVideoSyncSender::load(const string name)
{
    #ifdef TARGET_RASPBERRY_PI
    ofxOMXPlayerSettings settings;
    string videoPath = ofToDataPath(name, true);
    settings.videoPath = videoPath;
    settings.enableTexture = true;
    settings.enableLooping = true;
    settings.enableAudio = false;

    video.setup(settings);

    #else
    video.load(name);
    #endif
}


void ofxVideoSyncSender::setup(bool _localhost)
{
    bool bLoaded;    
    #ifdef TARGET_RASPBERRY_PI
    bLoaded = video.isOpen();
    #else
    bLoaded = video.isLoaded();
    #endif    
    if(!bLoaded) {
        ofLogError("ofxVideoSyncSender") << "Video needs to be loaded before setup().";
        return;
    }

    bUseLocalhost = _localhost;
    totalFrames = video.getTotalNumFrames();
    
    #ifdef TARGET_RASPBERRY_PI
    vidDuration = video.getDurationInSeconds();
    #else
    vidDuration = video.getDuration();
    #endif    
    ofLogNotice("ofxVideoSyncSender") << "Video duration: " << vidDuration;

    if (bUseLocalhost) {
        ofLogNotice("ofxVideoSyncSender") << "broadcasting on localhost";
        oscSender.setup("localhost", 12345);
    } else {
        string ip = getBroadcastIP();
        ofLogNotice("ofxVideoSyncSender") << "broadcasting on " << ip;
        oscSender.setup(ip, 12345);
    }
}

void ofxVideoSyncSender::play()
{
    #ifndef TARGET_RASPBERRY_PI
    video.play();
    #endif
}

void ofxVideoSyncSender::draw(float x, float y, float w, float h)
{
    video.draw(x, y, w, h);
}

void ofxVideoSyncSender::draw(float x, float y)
{
    #ifdef TARGET_RASPBERRY_PI
    video.draw(x,y,video.getWidth(),video.getHeight());
    #else
    video.draw(x,y);
    #endif
}

void ofxVideoSyncSender::update()
{
    #ifndef TARGET_RASPBERRY_PI
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
