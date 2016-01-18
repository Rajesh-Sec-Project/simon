//
// Created by remi on 18/01/16.
//

#include "soundmanager.h"
#include <QMediaPlayer>
#include <memory>
#include <QApplication>

namespace {
    std::unique_ptr<QMediaPlayer> _player;
}

std::string const SoundManager::menuMusic = "qrc:/simon/music_menu";
std::string const SoundManager::gameMusic = "qrc:/simon/music_game";
std::string const SoundManager::lostMusic = "qrc:/simon/music_loose";

void SoundManager::init() {
    _player = std::make_unique<QMediaPlayer>();
}

void SoundManager::setVolume(float volume) {
    if(_player) {
        _player->setVolume(static_cast<int>(volume * 100));
    }
}

void SoundManager::resume() {
    if(_player) {
        _player->play();
    }
}

void SoundManager::pause() {
    if(_player) {
        _player->pause();
    }
}

void SoundManager::playMusic(std::string const &str) {
    _player->setMedia(QUrl(str.c_str()));
    SoundManager::setVolume(1);
    SoundManager::resume();
}