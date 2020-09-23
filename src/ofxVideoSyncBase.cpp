/* Author: Pierre Proske (www.sensoryempire.com)
 * 
 * Date of Creation: September 2020
 * 
 */


#include "ofxVideoSyncBase.h"

//---------------------------------------------------------------------------
void ofxVideoSyncBase::load(const string name)
{
    #ifdef TARGET_RASPBERRY_PI
    ofxOMXPlayerSettings settings;
    string videoPath = ofToDataPath(name, true);
    settings.videoPath = videoPath;
    settings.enableTexture = true;
    settings.enableLooping = true;
    settings.enableAudio = false;
    settings.listener = this;

    video.setup(settings);

    #else
    video.load(name);
    #endif
}

void ofxVideoSyncBase::setup() {
    bool bLoaded;
    #ifdef TARGET_RASPBERRY_PI
    bLoaded = video.isOpen();
    #else
    bLoaded = video.isLoaded();
    #endif
    if(!bLoaded) {
        ofLogError("ofxVideoSyncBase") << "Video needs to be loaded before setup().";
        return;
    }

    totalFrames = video.getTotalNumFrames();

    #ifdef TARGET_RASPBERRY_PI
    vidDuration = video.getDurationInSeconds();
    #else
    vidDuration = video.getDuration();
    #endif

    ofLogVerbose("ofxVideoSyncBase") << "setup()";
}

//---------------------------------------------------------------------------
void ofxVideoSyncBase::play()
{
    #ifndef TARGET_RASPBERRY_PI
    video.play();
    #endif
}

//---------------------------------------------------------------------------
void ofxVideoSyncBase::draw(float x, float y, float w, float h)
{
    video.draw(x,y,w,h);
}

//---------------------------------------------------------------------------
void ofxVideoSyncBase::draw(float x, float y)
{
    #ifdef TARGET_RASPBERRY_PI
    video.draw(x,y,video.getWidth(),video.getHeight());
    #else
    video.draw(x,y);
    #endif
}

//---------------------------------------------------------------------------
void ofxVideoSyncBase::stop()
{
    video.stop();
}

//---------------------------------------------------------------------------
void ofxVideoSyncBase::close()
{
    video.close();
}

//---------------------------------------------------------------------------
void ofxVideoSyncBase::setVolume(float volume) {
     video.setVolume(volume);
}

//---------------------------------------------------------------------------
void ofxVideoSyncBase::setPaused(bool bPause) {
    video.setPaused(bPause);
}

//---------------------------------------------------------------------------
void ofxVideoSyncBase::togglePause() {
#ifdef TARGET_RASPBERRY_PI
    video.togglePause();
#else
    video.setPaused(!video.isPaused());
#endif
}

//--------------------------------------------------------
void ofxVideoSyncBase::setLoopState(ofLoopType state) {
    video.setLoopState(state);
}

//---------------------------------------------------------------------------
ofLoopType ofxVideoSyncBase::getLoopState() const {
#ifdef TARGET_RASPBERRY_PI
    if(video.isLoopingEnabled)
        return OF_LOOP_NORMAL
    else
        return OF_LOOP_NONE;
#else
    return video.getLoopState();
#endif
}

//---------------------------------------------------------------------------
ofTexture &	ofxVideoSyncBase::getTexture() {
#ifdef TARGET_RASPBERRY_PI
    return video.getTextureReference();
#else
    return video.getTexture();
#endif
}
