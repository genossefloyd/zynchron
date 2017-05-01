#include "maingui.h"
#include "ui_maingui.h"
#include "socketoutput.h"
#include "mqttoutput.h"
#include "deviceinfo.h"
#include "mwdevicecontrol.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

static MainGui* gui = NULL;

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

    std::cout << msg.toStdString() << std::endl;
    if(gui)
        emit gui->received_output(msgType+msg);
}

MainGui::MainGui(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MainGui),
    m_connector(this)
{
    m_ui->setupUi(this);

    MqttOutput* out = MqttOutput::instance();
    //out->init("127.0.0.1",1883);
    //out->init("192.168.178.62",1883);
    out->init("broker.mqttdashboard.com",1883);

    connect(this, SIGNAL(clicked_SendDummyData()), out, SLOT(toogleDummy()));
    connect(&m_connector, SIGNAL(updated()), this, SLOT(updateDeviceList()));

    gui = this;
    connect(this, SIGNAL(received_output(QString)), this, SLOT(addLogOutput(QString)));
    qInstallMessageHandler(messageOutputGUI);

}

MainGui::~MainGui()
{
    MqttOutput::instance()->shutdown();
    gui = NULL;
    delete m_ui;
}

void MainGui::updateDeviceList()
{
}

void MainGui::addLogOutput(QString msg)
{
    m_ui->output->append(msg);
}

void MainGui::on_butten_Search_clicked()
{
    m_connector.startDeviceDiscovery();
}

void MainGui::on_button_LED_clicked()
{

}

void MainGui::on_button_Data_clicked()
{

}

void MainGui::on_button_dummyData_clicked()
{
    emit clicked_SendDummyData();
}

void MainGui::on_button_Data_released()
{
    emit clicked_FetchData();
}

void MainGui::on_button_LED_released()
{
    qDebug() << "released";
    emit clicked_LED();
}
