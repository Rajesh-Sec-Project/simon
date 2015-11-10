#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>

using namespace std::literals;

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , _ui(std::make_unique<Ui::MainWindow>()) {
    _ui->setupUi(this);
    this->connect(_ui->myButton, SIGNAL(clicked()), this, SLOT(onClickClick()));
}

MainWindow::~MainWindow() {
}


void MainWindow::onClickClick() {
    std::cout << "Sending \"" << _ui->text->toPlainText().toUtf8().constData() << "\" to the network!" << std::endl;
}
