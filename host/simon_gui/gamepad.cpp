#include "gamepad.h"
#include "ui_gamepad.h"
#include <QKeyEvent>
#include <QToolButton>
#include <QDebug>

GamePad::GamePad(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::GamePad)
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->button_down, SIGNAL(clicked()), this, SLOT(M_down()));
    QObject::connect(m_ui->button_up, SIGNAL(clicked()), this, SLOT(M_up()));
    QObject::connect(m_ui->button_left, SIGNAL(clicked()), this, SLOT(M_left()));
    QObject::connect(m_ui->button_right, SIGNAL(clicked()), this, SLOT(M_right()));
    QObject::connect(m_ui->button_stop, SIGNAL(clicked()), this, SLOT(M_stop()));
}

GamePad::~GamePad()
{
    delete m_ui;
}

void GamePad::M_up()
{
    emit up();
}

void GamePad::M_down()
{
    emit down();
}

void GamePad::M_left()
{
    emit left();
}

void GamePad::M_right()
{
    emit right();
}

void GamePad::M_stop()
{
    emit stop();
}

void GamePad::keyPressEvent(QKeyEvent* evt)
{
    evt->ignore();

    switch (evt->key())
    {
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

    case Qt::Key_Escape:
        m_ui->button_stop->animateClick();
        emit stop();
        break;
    }
}
