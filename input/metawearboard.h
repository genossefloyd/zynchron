#ifndef METAWEARBOARD_H
#define METAWEARBOARD_H

#include <QObject>
#include <QBluetoothUuid>
#include <QLowEnergyService>
#include <QLowEnergyController>

#include "core/metawearboard.h"

typedef QMap<QBluetoothUuid,QLowEnergyService*> ServiceMap;

class metawearboard : public QObject
{
    Q_OBJECT
public:
    explicit metawearboard(QObject *parent = 0);
    virtual ~metawearboard();

    bool init(QLowEnergyController *controller );

    static const QBluetoothUuid METAWARE_SERVICE_UUID;
    static const QBluetoothUuid METAWARE_CHARACTERISTIC_UUID;

private:
    MblMwBtleConnection m_connection;

    MblMwMetaWearBoard*     m_board;
    QLowEnergyController*   m_control;
    QLowEnergyService*      m_mwService;

    static ServiceMap m_services;

    QLowEnergyDescriptor m_notificationDesc;

    void cleanup();

    static void write_gatt_char(const void* caller, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);

    static void read_gatt_char(const void* caller, const MblMwGattChar *characteristic);

    static void cb_IsInitialized(MblMwMetaWearBoard *board, int32_t status);

signals:

public slots:

private slots:
    void deviceConnected();
    void deviceDisconnected();
    void controllerError(QLowEnergyController::Error);

    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState s);

    void updateCharacteristic(QLowEnergyCharacteristic,QByteArray);
    void readCharacteristic(QLowEnergyCharacteristic,QByteArray);
    void writtenCharacteristic(QLowEnergyCharacteristic,QByteArray);

    void confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value);
    void confirmedDescriptorRead(const QLowEnergyDescriptor &d, const QByteArray &value);
};

QUuid HighLow2Uuid(const uint64_t high, const uint64_t low);
void Uuid2HighLow(QUuid uuid, uint64_t &high, uint64_t &low);

#endif // METAWEARBOARD_H
