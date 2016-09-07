#include "maingui.h"
#include "ui_maingui.h"
#include "socketoutput.h"
#include "deviceinfo.h"
#include "mwdevicecontrol.h"

#include <QBluetoothDeviceInfo>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

MainGui::MainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainGui),
    m_connector(this)
{
    ui->setupUi(this);

    connect(this, SIGNAL(output_added(QString)), this, SLOT(on_output_added(QString)));
    //qInstallMessageHandler(messageOutputGUI);

    connect(this, SIGNAL(clicked_SendDummyData()), SocketOutput::instance(), SLOT(toogleDummy()));
    connect(&m_connector, SIGNAL(updated()), this, SLOT(updateDeviceList()));
}

MainGui::~MainGui()
{
    delete ui;
}

void MainGui::updateDeviceList()
{
}

void MainGui::on_butten_Search_clicked()
{
    m_connector.startDeviceDiscovery();
}

void MainGui::on_button_LED_clicked()
{
    emit clicked_LED();
}

void MainGui::on_button_Data_clicked()
{
    emit clicked_FetchData();
}

void MainGui::on_button_dummyData_clicked()
{
    emit clicked_SendDummyData();
}

void MainGui::messageOutputGUI(QtMsgType type, const QMessageLogContext &/*context*/, const QString &msg)
{
    QString msgType;
    switch (type)
    {
    case QtDebugMsg:
        msgType = "";
        break;
    case QtInfoMsg:
        msgType = "Info: ";
        break;
    case QtWarningMsg:
        msgType = "Warning: ";
        break;
    case QtCriticalMsg:
        msgType = "Critical: ";
        break;
    case QtFatalMsg:
        msgType = "Fatal: ";
        break;
    }

    //emit gui->output_added(msg);
    std::cout << msgType.toStdString() << msg.toStdString();
}

void MainGui::on_output_added(QString msg)
{
    ui->textBrowser->append(msg);
}
