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

#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <memory>
#include <functional>

namespace Ui {
    class SettingsView;
}

class SettingsView : public QWidget {
    Q_OBJECT

public:
    explicit SettingsView(QWidget* parent = 0);
    ~SettingsView();

    static void setMusicToggledCallback(std::function<void(bool)> const& callback) {
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
