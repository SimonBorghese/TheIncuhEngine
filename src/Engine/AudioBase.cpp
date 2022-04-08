#include <AudioBase.hpp>

AudioBase::AudioBase()
{
    base.init();
}

AudioBase::~AudioBase()
{
    base.deinit();
}

void AudioBase::playSoundGlobal(const char *musicPath, int loop){
    globalSound.load(musicPath);
    globalSound.setLooping(loop);
    base.play(globalSound);
}
