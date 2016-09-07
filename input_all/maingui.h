#ifndef MAINGUI_H
#define MAINGUI_H

#include <QtGlobal>
#include <QWidget>
#include <QMutex>

#include "bluetoothconnector.h"

namespace Ui {
class MainGui;
}

class MainGui : public QWidget
{
    Q_OBJECT

public:
    explicit MainGui(QWidget *parent = 0);
    ~MainGui();

private:
    Ui::MainGui *ui;
    BluetoothConnector m_connector;

    //static QTextBrowser* output;
    //static QMutex m_outputMutex;

    static void messageOutputGUI(QtMsgType type, const QMessageLogContext &context, const QString &msg);

signals:
    void clicked_LED();
    void clicked_FetchData();
    void clicked_SendDummyData();
    void output_added(QString msg);

public slots:
    void updateDeviceList();

private slots:
    void on_butten_Search_clicked();
    void on_button_LED_clicked();
    void on_button_Data_clicked();
    void on_button_dummyData_clicked();
    void on_output_added(QString msg);
};

#endif // MAINGUI_H
