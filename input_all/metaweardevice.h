#ifndef METAWEARDEVICE_H
#define METAWEARDEVICE_H

#include "deviceinfo.h"
#include <QBluetoothUuid>
#include <QLowEnergyService>
#include <QLowEnergyController>

#include "core/metawearboard.h"
#include "core/cpp/metawearboard_def.h"
#include "core/data.h"

typedef QMap<QBluetoothUuid,QLowEnergyService*> ServiceMap;

class MetaWearDevice : public DeviceInfo
{
    Q_OBJECT
public:
    enum Signal {
        SWITCH,
        ACCELEROMETER,
        AMBIENT_LIGHT,
        GYRO
    };

    explicit MetaWearDevice(const QBluetoothDeviceInfo &deviceInfo,
                            QObject *parent = Q_NULLPTR);
    virtual ~MetaWearDevice();

    bool init();

private:
    static const MblMwBtleConnection CONNECTION;

    struct MblMwMetaWearBoardCustom : public MblMwMetaWearBoard
    {
        QLowEnergyController*       controller;
        ServiceMap                  services;
        QLowEnergyService*          mwService;
        bool                        reconnect;
        State*                      state;
    };

    MblMwMetaWearBoardCustom*   m_board;

    static MblMwMetaWearBoardCustom* mwbc_create(QLowEnergyController *controller = NULL, State* statePtr = NULL);
    static void mwbc_disconnect(MblMwMetaWearBoardCustom* board);

    static void is_initialized(MblMwMetaWearBoard *board, int32_t status);

    static void write_gatt_char(const void* caller, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);
    static void read_gatt_char(const void* caller, const MblMwGattChar *characteristic);

    static void handle_data(const MblMwData* data, Signal signal);
    static void acc_handler(const MblMwData* data) { handle_data(data, Signal::ACCELEROMETER); }

signals:

public slots:
    void toogleLED();
    void fetchData();

private slots:
    void deviceConnected();
    void deviceDisconnected();
    void controllerError(QLowEnergyController::Error);

    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState s);

    void updateCharacteristic(QLowEnergyCharacteristic,QByteArray);
    void readCharacteristic(QLowEnergyCharacteristic,QByteArray);

    void confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value);
    void confirmedDescriptorRead(const QLowEnergyDescriptor &d, const QByteArray &value);
};

QUuid HighLow2Uuid(const uint64_t high, const uint64_t low);
void Uuid2HighLow(QUuid uuid, uint64_t &high, uint64_t &low);

#endif // METAWEARDEVICE_H
