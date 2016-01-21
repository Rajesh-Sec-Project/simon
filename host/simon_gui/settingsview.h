#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <memory>
#include <functional>

namespace Ui {
class SettingsView;
}

class SettingsView : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsView(QWidget* parent = 0);
    ~SettingsView();

    static void setMusicToggledCallback(std::function<void(bool)> const &callback) {
        m_callback = callback;
    }

    static bool soundEnabled();
    static bool musicEnabled();

private slots:
    void M_back();
    void M_setSound(bool yes, bool apply = false);
    void M_setMusic(bool yes, bool apply = false);

private:
    std::unique_ptr<Ui::SettingsView> m_ui;
    static std::function<void(bool)> m_callback;
};

#endif // SETTINGSVIEW_H
