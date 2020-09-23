/* Author: Pierre Proske (www.sensoryempire.com)
 * 
 * Date of Creation: September 2020
 * 
 */

#pragma once

#include "ofMain.h"

#ifdef TARGET_RASPBERRY_PI
#include "ofxOMXPlayer.h"
#endif

class ofxVideoSyncBase {
public:
    ~ofxVideoSyncBase() { ofLogNotice() << "Base class destructor called"; }
    void load(const std::string video);
    virtual void setup();
    virtual void update() = 0;

    void play();
    void draw(float x, float y, float w, float h);
    void draw(float x, float y);
    void stop();
    void close();
    void setVolume(float volume);
    void setPaused(bool bPause);
    void togglePause();
    void setLoopState(ofLoopType state);
    virtual ofLoopType getLoopState() const;
    ofTexture& getTexture();

#ifdef TARGET_RASPBERRY_PI
    ofxOMXPlayer* getVideoPlayerPtr()
    {
        return &video;
    }
#else
    ofVideoPlayer* getVideoPlayerPtr()
    {
        return &video;
    }
#endif

protected:
#ifdef TARGET_RASPBERRY_PI
    ofxOMXPlayer video;
#else
    ofVideoPlayer video;
#endif
    int totalFrames;
    float vidDuration;
    ofXml xml;

};
