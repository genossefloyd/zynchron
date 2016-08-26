#ifndef MAINGUI_H
#define MAINGUI_H

#include <QWidget>
#include <QStandardItemModel>

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
    QStandardItemModel *m_deviceListModel;

    BluetoothConnector m_connector;

signals:
    void clicked_LED();

public slots:
    void updateDeviceList();

private slots:
    void on_pushButton_clicked();

    void on_deviceList_doubleClicked(const QModelIndex &index);

    void on_button_LED_clicked();
};

#endif // MAINGUI_H
