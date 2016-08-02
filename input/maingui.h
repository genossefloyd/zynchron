#ifndef MAINGUI_H
#define MAINGUI_H

#include <QWidget>

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

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainGui *ui;
    BluetoothConnector connector;
};

#endif // MAINGUI_H
