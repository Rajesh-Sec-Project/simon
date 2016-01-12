#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "lcomm/gamepad_packet.h"
#include "lcomm/gamepad_position_packet.h"
#include "lcomm/log_packet.h"
#include "lcomm/info_packet.h"
#include "lcomm/score_packet.h"
#include "commmanager.h"

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <sstream>

using namespace std::literals;

GameWindow::GameWindow(QWidget* parent)
        : QMainWindow(parent)
        , m_ui(std::make_unique<Ui::GameWindow>()) {

    m_ui->setupUi(this);

    // M_stop();
}

GameWindow::~GameWindow() {
}


QStackedWidget& GameWindow::stackedWidget() {
    return *m_ui->stackedWidget;
}
