#include "addwindow.h"
#include "ui_addwindow.h"

addWindow::addWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addWindow)
{
    ui->setupUi(this);
}

addWindow::~addWindow()
{
    delete ui;
}
