#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <memory>

namespace Ui {
class SettingsView;
}

class SettingsView : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsView(QWidget* parent = 0);
    ~SettingsView();

    static bool soundEnabled();
    static bool musicEnabled();

private slots:
    void M_back();
    void M_setSound(bool yes, bool apply = false);
    void M_setMusic(bool yes, bool apply = false);

private:
    std::unique_ptr<Ui::SettingsView> m_ui;
};

#endif // SETTINGSVIEW_H
