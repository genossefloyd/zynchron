#ifndef METAWEARDEVICE_H
#define METAWEARDEVICE_H

#include "bluetoothdevice.h"
#include "abstractoutput.h"

#include "core/metawearboard.h"
#include "core/cpp/metawearboard_def.h"
#include "core/data.h"

#include <map>

class MetaWearDevice : public BluetoothDevice
{
public:
    enum Signal {
        SWITCH,
        ACCELEROMETER,
        AMBIENT_LIGHT,
        GYRO
    };

    explicit MetaWearDevice(BluetoothDevice & device, AbstractOutput* output);
    virtual ~MetaWearDevice();

    bool initialize();

private:
    static const MblMwBtleConnection CONNECTION;

    struct MblMwMetaWearBoardCustom : public MblMwMetaWearBoard
    {
        MetaWearDevice*             parent;
        uint8_t                     datastreamid;
        bool                        reconnect;
        bool                        fetchingdata;
    };

    MblMwMetaWearBoardCustom*   m_board;
    AbstractOutput* m_output;

    static MblMwMetaWearBoardCustom* mwbc_create(MetaWearDevice* parent);
    static void mwbc_disconnect(MblMwMetaWearBoardCustom* board);

    static void is_initialized(MblMwMetaWearBoard *board, int32_t status);

    static void write_gatt_char(const void* caller, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);
    static void read_gatt_char(const void* caller, const MblMwGattChar *characteristic);
    static void gatt_notification_cb(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data);

    static void handle_data(const MblMwMetaWearBoardCustom* caller, const MblMwData* data, Signal signal);
    static void acc_handler(const void* caller, const MblMwData* data)
    {
        handle_data((MblMwMetaWearBoardCustom*)caller, data, Signal::ACCELEROMETER);
    }

    void updateCharacteristic(const gattlib_characteristic_t* gattChar, const uint8_t* value, uint8_t length);

public:
    void toogleLED();
    void fetchData();
};

uuid_t HighLow2Uuid(const uint64_t high, const uint64_t low);
void Uuid2HighLow(uuid_t uuid, uint64_t &high, uint64_t &low);

#endif // METAWEARDEVICE_H
