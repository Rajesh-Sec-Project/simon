/*
 * Copyright (c) 2016 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
    if(SettingsView::musicEnabled()) {
        SoundManager::resume();
    }
}

void SoundManager::playSound(const std::string& str) {
    if(SettingsView::soundEnabled()) {
        _soundPlayer->setMedia(QUrl(str.c_str()));
        _soundPlayer->setVolume(static_cast<int>(100));
        _soundPlayer->play();
    }
}
