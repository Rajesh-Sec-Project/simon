#include "Mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _ui(std::make_unique<Ui::MainWindow>()) {
    _ui->setupUi(this);
}

MainWindow::~MainWindow() {}
