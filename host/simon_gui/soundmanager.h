//
// Created by remi on 18/01/16.
//

#ifndef SIMON_SOUNDMANAGER_H
#define SIMON_SOUNDMANAGER_H

#include <string>

namespace SoundManager {
    void init();

    void setVolume(float volume);

    void pause();
    void resume();
    void playMusic(std::string const &str);
    void playSound(std::string const &str);

    extern std::string const menuMusic, gameMusic, lostMusic, winSound, loseSound, goodSound;
}

#endif //SIMON_GAMEMANAGER_H
