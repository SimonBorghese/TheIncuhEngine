#ifndef AUDIOBASE_HPP
#define AUDIOBASE_HPP
#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>

class AudioBase
{
    public:
        AudioBase();
        virtual ~AudioBase();

        void playSoundGlobal(const char *musicPath, int loop);

    protected:

    private:
        SoLoud::Soloud base;
        SoLoud::Wav globalSound;
};

#endif // AUDIOBASE_HPP
