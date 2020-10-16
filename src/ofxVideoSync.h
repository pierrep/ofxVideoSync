/* Author: Pierre Proske (www.sensoryempire.com)
 * 
 * Date of Creation: September 2020
 * 
 */

#pragma once
#include "ofMain.h"
#include "ofxVideoSyncBase.h"
#include "ofVideoBaseTypes.h"

enum SyncType : short{
    SYNC_NONE=0x00,
    SYNC_SENDER=0x01,
    SYNC_RECEIVER=0x02,
};

class ofxVideoSync {
public:
    ofxVideoSync();
    void load(const std::string video);
    void play();
    void draw(float x, float y, float w, float h);
    void draw(float x, float y);
    ofTexture &	getTexture();
    void stop();
    void close();
    void update();    
    void setVolume(float volume);
    void setLoopState(ofLoopType state);
    ofLoopType getLoopState() const;
    void setType(SyncType _type);
    void setPaused(bool bPause);
    void togglePause();
    bool isPaused() const;
    void loadSettings();
    void enableSync();
    void disableSync();

#ifdef TARGET_RASPBERRY_PI
    ofxOMXPlayer* getVideoPlayerPtr();
    void setPlayerSettings(ofxOMXPlayerSettings settings) {playerSettings = settings;}
#else
    ofVideoPlayer* getVideoPlayerPtr();
#endif

private:
#ifdef TARGET_RASPBERRY_PI
    ofxOMXPlayerSettings playerSettings;
#endif    
	std::shared_ptr<ofxVideoSyncBase>		player;
    void setPlayer(std::shared_ptr<ofxVideoSyncBase> newPlayer);
    SyncType sync_type;
    ofXml xml;
};
