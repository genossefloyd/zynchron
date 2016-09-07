#include "mwdevicecontrol.h"
#include "ui_mwdevicecontrol.h"

MwDeviceControl::MwDeviceControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MwDeviceControl)
{
    ui->setupUi(this);
}

MwDeviceControl::~MwDeviceControl()
{
    delete ui;
}
