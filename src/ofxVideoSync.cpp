#include "ofxVideoSync.h"
#include "ofxVideoSyncReceiver.h"
#include "ofxVideoSyncSender.h"

//---------------------------------------------------------------------------
ofxVideoSync::ofxVideoSync()
{
    #ifdef TARGET_RASPBERRY_PI
    playerSettings.enableTexture = true;
    playerSettings.enableLooping = true;
    playerSettings.enableAudio = false;    
#endif
}

//---------------------------------------------------------------------------
void ofxVideoSync::loadSettings()
{
    if (!xml.load("sync_settings.xml")) {
        ofLogWarning() << "Couldn't load settings file. We will create it instead.";
    }
    auto settings = xml.getChild("settings");
    if (!settings) {
        settings = xml.appendChild("settings");
    }

    ofXml entry = settings.findFirst("sync_type");
    if (!entry) {
        // remove any possible malformed tag
        settings.removeChild("sync_type");
        settings.appendChild("sync_type").set("Receiver");
    }
    auto new_sync_type = xml.findFirst("//sync_type");
    if (new_sync_type) {
        string value = new_sync_type.getValue();
        if(value == "Sender") {
            sync_type = SyncType::SYNC_SENDER;
        }
        else {
            sync_type = SyncType::SYNC_RECEIVER;
        }
        ofLogNotice() << "Set sync type to: " << value;
    }

    if (!xml.save("sync_settings.xml")) {
        ofLogError() << "Couldn't save settings.xml";
    }
}

//---------------------------------------------------------------------------
void ofxVideoSync::load(const std::string video)
{
    if(sync_type == SyncType::SYNC_NONE) {
        loadSettings();
    }
    if( !player ){
        if(sync_type == SyncType::SYNC_RECEIVER) {
            setPlayer(std::make_shared<ofxVideoSyncReceiver>());
            ofLogNotice() << "Create Sync Receiver";
        } else {
            setPlayer(std::make_shared<ofxVideoSyncSender>());
            ofLogNotice() << "Create Sync Sender";
        }
    }

#ifdef TARGET_RASPBERRY_PI
    player->setPlayerSettings(playerSettings);
#endif
    player->load(video);
    player->setup();

}

//---------------------------------------------------------------------------
void ofxVideoSync::play() {
    player->play();
}

//---------------------------------------------------------------------------
void ofxVideoSync::stop() {
    player->stop();
}

//---------------------------------------------------------------------------
void ofxVideoSync::close() {
    player->close();
}

//---------------------------------------------------------------------------
void ofxVideoSync::draw(float x, float y, float w, float h) {
    player->draw(x,y,w,h);
}

//---------------------------------------------------------------------------
void ofxVideoSync::draw(float x, float y) {
    player->draw(x,y);
}

//---------------------------------------------------------------------------
void ofxVideoSync::update() {
    player->update();
}

//---------------------------------------------------------------------------
void ofxVideoSync::setPaused(bool _bPause){
    if( player ){
        player->setPaused(_bPause);
    }
}

//---------------------------------------------------------------------------
void ofxVideoSync::togglePause(){
    if( player ){
        player->togglePause();
    }
}

//--------------------------------------------------------
void ofxVideoSync::setVolume(float volume){
    if( player ){
        if ( volume > 1.0f ){
            ofLogWarning("ofVideoPlayer") << "setVolume(): expected range is 0-1, limiting requested volume " << volume << " to 1.0";
            volume = 1.0f;
        }
        player->setVolume(volume);
    }
}

//--------------------------------------------------------
void ofxVideoSync::setLoopState(ofLoopType state){
    if( player ){
        player->setLoopState(state);
    }
}

//---------------------------------------------------------------------------
ofLoopType ofxVideoSync::getLoopState() const{
    if( player ){
        return player->getLoopState();
    }else{
        return OF_LOOP_NONE;
    }
}

//---------------------------------------------------------------------------
void ofxVideoSync::setType(SyncType _type)
{
    sync_type = _type;
    ofLogNotice() << "Manually set sync type...";
}

//---------------------------------------------------------------------------
void ofxVideoSync::setPlayer(std::shared_ptr<ofxVideoSyncBase> newPlayer) {
    player = newPlayer;
}

//---------------------------------------------------------------------------
ofTexture & ofxVideoSync::getTexture(){
    return player->getTexture();
}

//---------------------------------------------------------------------------
#ifdef TARGET_RASPBERRY_PI
ofxOMXPlayer* ofxVideoSync::getVideoPlayerPtr() {
#else
ofVideoPlayer* ofxVideoSync::getVideoPlayerPtr() {
#endif
    if(player) {
        return player->getVideoPlayerPtr();
    }
    return nullptr;
}
