#include "settingsview.h"
#include "ui_settingsview.h"
#include "lcomm/info_packet.h"
#include "viewmanager.h"
#include "commmanager.h"
#include <QSettings>

using namespace lcomm;

SettingsView::SettingsView(QWidget *parent) :
    QWidget(parent),
    m_ui(std::make_unique<Ui::SettingsView>())
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->backButton, SIGNAL(clicked()), this, SLOT(M_back()));

    M_setSound(soundEnabled(), true);
    M_setSound(musicEnabled(), true);

    QObject::connect(m_ui->soundButton, SIGNAL(toggled(bool)), this, SLOT(M_setSound(bool)));
    QObject::connect(m_ui->musicButton, SIGNAL(toggled(bool)), this, SLOT(M_setMusic(bool)));
}

SettingsView::~SettingsView()
{

}

bool SettingsView::musicEnabled()
{
    QSettings settings;
    return settings.value("musicEnabled", true).toBool();
}

bool SettingsView::soundEnabled()
{
    QSettings settings;
    return settings.value("soundEnabled", true).toBool();
}

void SettingsView::M_setSound(bool yes, bool apply)
{
    if (apply)
        m_ui->soundButton->setChecked(yes);
    m_ui->soundButton->setText("Sound : " + QString(yes ? "yes" : "no"));

    QSettings settings;
    settings.value("soundEnabled", yes);
}

void SettingsView::M_setMusic(bool yes, bool apply)
{
    if (apply)
        m_ui->musicButton->setChecked(yes);
    m_ui->musicButton->setText("Music : " + QString(yes ? "yes" : "no"));

    QSettings settings;
    settings.value("musicEnabled", yes);
}

void SettingsView::M_back() {
    ViewManager::switchToMainMenu();
}
