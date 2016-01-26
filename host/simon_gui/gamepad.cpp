/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
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

#include "gamepad.h"
#include "ui_gamepad.h"
#include <QKeyEvent>
#include <QToolButton>
#include <QDebug>

GamePad::GamePad(QWidget* parent)
        : QWidget(parent)
        , m_ui(std::make_unique<Ui::GamePad>()) {
    m_ui->setupUi(this);

    QObject::connect(m_ui->button_down, SIGNAL(clicked()), this, SLOT(M_down()));
    QObject::connect(m_ui->button_up, SIGNAL(clicked()), this, SLOT(M_up()));
    QObject::connect(m_ui->button_left, SIGNAL(clicked()), this, SLOT(M_left()));
    QObject::connect(m_ui->button_right, SIGNAL(clicked()), this, SLOT(M_right()));
}

GamePad::~GamePad() {
}

void GamePad::M_up() {
    emit up();
}

void GamePad::M_down() {
    emit down();
}

void GamePad::M_left() {
    emit left();
}

void GamePad::M_right() {
    emit right();
}

void GamePad::keyPressEvent(QKeyEvent* evt) {
    evt->ignore();

    switch(evt->key()) {
        case Qt::Key_Z:
            m_ui->button_up->animateClick();
            emit up();
            break;

        case Qt::Key_W:
            m_ui->button_down->animateClick();
            emit down();
            break;

        case Qt::Key_Q:
            m_ui->button_left->animateClick();
            emit left();
            break;

        case Qt::Key_D:
            m_ui->button_right->animateClick();
            emit right();
            break;
    }
}
