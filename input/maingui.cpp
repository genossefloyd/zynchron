#include "maingui.h"
#include "ui_maingui.h"

MainGui::MainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainGui)
{
    ui->setupUi(this);
}

MainGui::~MainGui()
{
    delete ui;
}

void MainGui::on_pushButton_clicked()
{
    connector.startDeviceDiscovery();
}
