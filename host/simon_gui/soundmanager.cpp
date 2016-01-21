//
// Created by remi on 18/01/16.
//

#include "soundmanager.h"
#include <QMediaPlayer>
#include <memory>
#include <QApplication>
#include "settingsview.h"

namespace {
    std::unique_ptr<QMediaPlayer> _player;
    std::unique_ptr<QMediaPlayer> _soundPlayer;
}

std::string const SoundManager::menuMusic = "qrc:/simon/music_menu";
std::string const SoundManager::gameMusic = "qrc:/simon/music_game";
std::string const SoundManager::lostMusic = "qrc:/simon/music_loose";
std::string const SoundManager::goodSound = "qrc:/simon/sound_good";
std::string const SoundManager::winSound = "qrc:/simon/sound_win";
std::string const SoundManager::loseSound = "qrc:/simon/sound_loose";

void SoundManager::init() {
    _player = std::make_unique<QMediaPlayer>();
    _soundPlayer = std::make_unique<QMediaPlayer>();
    SettingsView::setMusicToggledCallback([](bool enabled) {
        if(enabled) {
            SoundManager::resume();
        } else {
            SoundManager::pause();
        }
    });
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

void SoundManager::playMusic(std::string const& str) {
    _player->setMedia(QUrl(str.c_str()));
    SoundManager::setVolume(0.5);
    SoundManager::resume();
}

void SoundManager::playSound(const std::string& str) {
    if(SettingsView::soundEnabled()) {
        _soundPlayer->setMedia(QUrl(str.c_str()));
        _soundPlayer->setVolume(static_cast<int>(100));
        _soundPlayer->play();
    }
}