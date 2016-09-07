#ifndef MWDEVICECONTROL_H
#define MWDEVICECONTROL_H

#include <QWidget>

namespace Ui {
class MwDeviceControl;
}

class MwDeviceControl : public QWidget
{
    Q_OBJECT

public:
    explicit MwDeviceControl(QWidget *parent = 0);
    ~MwDeviceControl();

private:
    Ui::MwDeviceControl *ui;
};

#endif // MWDEVICECONTROL_H
