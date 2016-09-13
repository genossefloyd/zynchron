#include "maingui.h"
#include "ui_maingui.h"
#include "socketoutput.h"
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

    connect(this, SIGNAL(clicked_SendDummyData()), SocketOutput::instance(), SLOT(toogleDummy()));
    connect(&m_connector, SIGNAL(updated()), this, SLOT(updateDeviceList()));

    gui = this;
    connect(this, SIGNAL(received_output(QString)), this, SLOT(addLogOutput(QString)));
    qInstallMessageHandler(messageOutputGUI);
}

MainGui::~MainGui()
{
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
