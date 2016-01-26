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

#include "settingsview.h"
#include "ui_settingsview.h"
#include "lcomm/info_packet.h"
#include "viewmanager.h"
#include "commmanager.h"
#include <QSettings>

using namespace lcomm;

std::function<void(bool)> SettingsView::m_callback;

SettingsView::SettingsView(QWidget* parent)
        : QWidget(parent)
        , m_ui(std::make_unique<Ui::SettingsView>()) {
    m_ui->setupUi(this);

    QObject::connect(m_ui->backButton, SIGNAL(clicked()), this, SLOT(M_back()));

    M_setSound(soundEnabled(), true);
    M_setMusic(musicEnabled(), true);

    QObject::connect(m_ui->soundButton, SIGNAL(toggled(bool)), this, SLOT(M_setSound(bool)));
    QObject::connect(m_ui->musicButton, SIGNAL(toggled(bool)), this, SLOT(M_setMusic(bool)));
}

SettingsView::~SettingsView() {
}

bool SettingsView::musicEnabled() {
    QSettings settings;
    return settings.value("musicEnabled").toBool();
}

bool SettingsView::soundEnabled() {
    QSettings settings;
    return settings.value("soundEnabled").toBool();
}

void SettingsView::M_setSound(bool yes, bool apply) {
    if(apply)
        m_ui->soundButton->setChecked(yes);
    m_ui->soundButton->setText("Sound : " + QString(yes ? "yes" : "no"));

    QSettings settings;
    settings.setValue("soundEnabled", yes);
}

void SettingsView::M_setMusic(bool yes, bool apply) {
    if(apply)
        m_ui->musicButton->setChecked(yes);
    m_ui->musicButton->setText("Music : " + QString(yes ? "yes" : "no"));

    if(m_callback) {
        m_callback(yes);
    }

    QSettings settings;
    settings.setValue("musicEnabled", yes);
}

void SettingsView::M_back() {
    ViewManager::switchToMainMenu();
}
