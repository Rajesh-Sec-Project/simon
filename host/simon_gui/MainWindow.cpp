#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>

using namespace std::literals;

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , _ui(std::make_unique<Ui::MainWindow>()) {
    _ui->setupUi(this);

    m_gamepad = new GamePad();
    m_gamepad->show();
}

MainWindow::~MainWindow() {
    delete m_gamepad;
}
