#include "metaweardevice.h"
#include "maingui.h"
#include "messages.h"
#include "socketoutput.h"

#include "core/cpp/constant.h"
#include "core/status.h"
#include "core/settings.h"
#include "core/module.h"
#include "core/datasignal.h"
#include "core/types.h"
#include "sensor/accelerometer.h"
#include "sensor/accelerometer_bosch.h"
#include "sensor/accelerometer_mma8452q.h"
#include "peripheral/led.h"

#include <sstream>
#include <string>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

static const QBluetoothUuid METAWARE_SERVICE_UUID = HighLow2Uuid(METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_high,
                                                    METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_low);
static const QBluetoothUuid METAWARE_CHARACTERISTIC_UUID = HighLow2Uuid(METAWEAR_SERVICE_NOTIFY_CHAR.uuid_high,
                                                    METAWEAR_SERVICE_NOTIFY_CHAR.uuid_low);

const MblMwBtleConnection MetaWearDevice::CONNECTION = { MetaWearDevice::write_gatt_char, MetaWearDevice::read_gatt_char };

QUuid HighLow2Uuid(const uint64_t high, const uint64_t low)
{
    char buffer[32];
    memset(buffer,0,32);
    sprintf(buffer, "%016" PRIx64 "%016" PRIx64, high, low);

    unsigned long p0;
    int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

    sscanf(buffer, "%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
        &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

    QUuid guid(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);

    return guid;
}

void Uuid2HighLow(QUuid uuid, uint64_t &high, uint64_t &low)
{
    std::string hx = uuid.toString().toStdString();

    for (size_t i = 0; i < hx.size(); i++)
    {
        if (hx[i] == '-' || hx[i] == '{' || hx[i] == '}') hx[i] = ' ';
    }

    std::istringstream iss(hx);
    std::string c1, c2, p1, p2, p3, p4, p5;
    iss >> p1 >> p2 >> p3 >> p4 >> p5;
    c1 = p1 + p2 + p3;
    c2 = p4 + p5;
    sscanf(c1.c_str(), "%llx", &high);
    sscanf(c2.c_str(), "%llx", &low);
}

MetaWearDevice::MetaWearDevice(const QBluetoothDeviceInfo &deviceInfo, QObject *parent)
    : DeviceInfo(deviceInfo, parent)
    , m_board(NULL)
{
    m_board = mwbc_create( this, QLowEnergyController::createCentral(deviceInfo, this), &m_state );
}

MetaWearDevice::~MetaWearDevice()
{
    m_board->reconnect = false;
    mwbc_disconnect(m_board);

    delete m_board->controller;
    m_board->controller = NULL;

    mbl_mw_metawearboard_free(m_board);
    m_board = NULL;
}

MetaWearDevice::MblMwMetaWearBoardCustom* MetaWearDevice::mwbc_create(MetaWearDevice* parent, QLowEnergyController* controller, State* statePtr)
{
    MblMwMetaWearBoardCustom* board = new MblMwMetaWearBoardCustom();
    memcpy(&board->btle_conn, &CONNECTION, sizeof(MblMwBtleConnection));
    board->controller = controller;
    board->reconnect = true;
    board->mwService = NULL;
    board->state = statePtr;
    board->parent = parent;
    return board;
}

void MetaWearDevice::is_initialized(MblMwMetaWearBoard* caller, int32_t status)
{
    MblMwMetaWearBoardCustom* board = (MblMwMetaWearBoardCustom*)caller;
    qDebug() << "Board status: " << status;

    if(status != MBL_MW_STATUS_ERROR_TIMEOUT)
    {
        if(board->state)
            *(board->state) = Connected;

        MblMwLedPattern pattern;
        mbl_mw_led_load_preset_pattern(&pattern, MBL_MW_LED_PRESET_BLINK);
        pattern.repeat_count = 3;

        MblMwLedColor color = MBL_MW_LED_COLOR_RED;
        if(status == MBL_MW_STATUS_OK)
        {
            color = MBL_MW_LED_COLOR_GREEN;
        }
        mbl_mw_led_write_pattern(board, &pattern, color);
        mbl_mw_led_play(board);
    }
    else
    {
        mwbc_disconnect(board);
    }
}

void MetaWearDevice::mwbc_disconnect(MblMwMetaWearBoardCustom *board)
{
    if( board->mwService )
    {
        QLowEnergyCharacteristic mwChar = board->mwService->characteristic(METAWARE_CHARACTERISTIC_UUID);
        QLowEnergyDescriptor notificationDesc = mwChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);

        if( notificationDesc.isValid() )
        {
            board->mwService->writeDescriptor(notificationDesc, QByteArray::fromHex("0000"));
            board->mwService = NULL;
        }
    }
    qDeleteAll(board->services);
    board->services.clear();

    mbl_mw_metawearboard_tear_down(board);
    board->controller->disconnectFromDevice();
    if(board->state)
        *(board->state) = Disconnected;
}

void MetaWearDevice::write_gatt_char(const void* caller, const MblMwGattChar *characteristic,
        const uint8_t *value, uint8_t length)
{
    MblMwMetaWearBoardCustom* board = (MblMwMetaWearBoardCustom*)caller;
    QBluetoothUuid uuidService = HighLow2Uuid(characteristic->service_uuid_high,characteristic->service_uuid_low);
    QBluetoothUuid uuidChar = HighLow2Uuid(characteristic->uuid_high,characteristic->uuid_low);

    ServiceMap::iterator itService = board->services.find(uuidService);
    if(board->services.end() != itService)
    {
        QLowEnergyCharacteristic qtCharacterstic = itService.value()->characteristic(uuidChar);
        QByteArray newValue((const char*)value,length);

        qDebug() << "Writting to GATT " << qtCharacterstic.uuid().toString() << " - value: " << newValue.toHex();
        itService.value()->writeCharacteristic(qtCharacterstic,newValue);
    }
}

void MetaWearDevice::read_gatt_char(const void* caller, const MblMwGattChar *characteristic)
{
    MblMwMetaWearBoardCustom* board = (MblMwMetaWearBoardCustom*)caller;
    QBluetoothUuid uuidService = HighLow2Uuid(characteristic->service_uuid_high,characteristic->service_uuid_low);
    QBluetoothUuid uuidChar = HighLow2Uuid(characteristic->uuid_high,characteristic->uuid_low);

    ServiceMap::iterator itService = board->services.find(uuidService);
    if(board->services.end() != itService)
    {
        QLowEnergyCharacteristic qtCharacterstic = itService.value()->characteristic(uuidChar);
        if(qtCharacterstic.isValid())
        {
            itService.value()->readCharacteristic(qtCharacterstic);
        }
    }
}

bool MetaWearDevice::init()
{
    QLowEnergyController* controller = m_board->controller;
    connect(controller, SIGNAL(connected()),
            this, SLOT(deviceConnected()));
    connect(controller, SIGNAL(disconnected()),
            this, SLOT(deviceDisconnected()));

    connect(controller, SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this, SLOT(serviceDiscovered(QBluetoothUuid)));
    connect(controller, SIGNAL(discoveryFinished()),
            this, SLOT(serviceScanDone()));

    connect(controller, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(controllerError(QLowEnergyController::Error)));

    m_state = Connecting;
    controller->connectToDevice();

    return true;
}

void MetaWearDevice::deviceConnected()
{
    m_board->controller->discoverServices();
}

void MetaWearDevice::deviceDisconnected()
{
    qWarning() << "MetaWearDevice disconnected";

    if(m_board->reconnect)
    {
        QLowEnergyController* controller = m_board->controller;
        mbl_mw_metawearboard_free(m_board);

        m_board = mwbc_create(this, controller, &m_state);
        m_board->reconnect = false;

        controller->connectToDevice();
    }
}

void MetaWearDevice::controllerError(QLowEnergyController::Error error)
{
    qDebug() << "Cannot connect to remote device.";
    qWarning() << "Controller Error:" << error;
}

void MetaWearDevice::serviceDiscovered(const QBluetoothUuid &gatt)
{
    if( !m_board->services.contains(gatt) )
    {
        qDebug() << "New service discovered: " << gatt.toString();

        QLowEnergyService* service = m_board->controller->createServiceObject(gatt, this);

        connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));

        connect(service, SIGNAL(characteristicRead(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(readCharacteristic(QLowEnergyCharacteristic,QByteArray)));
        connect(service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(updateCharacteristic(QLowEnergyCharacteristic,QByteArray)));

        m_board->services[gatt] = service;

        if( gatt == METAWARE_SERVICE_UUID)
        {
            m_board->mwService = service;

            connect(service, SIGNAL(descriptorRead(QLowEnergyDescriptor,QByteArray)),
                    this, SLOT(confirmedDescriptorRead(QLowEnergyDescriptor,QByteArray)));
            connect(service, SIGNAL(descriptorWritten(QLowEnergyDescriptor,QByteArray)),
                    this, SLOT(confirmedDescriptorWrite(QLowEnergyDescriptor,QByteArray)));
        }
    }
}

void MetaWearDevice::serviceScanDone()
{
    qDebug() << "Service scan done ";

    ServiceMap::iterator it = m_board->services.begin();
    while(it != m_board->services.end())
    {
        it.value()->discoverDetails();
        ++it;
    }
}

void MetaWearDevice::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Service State changed to: "<< s ;
    static int count = 0;
    if(QLowEnergyService::DiscoveringServices == s)
    {
        count = 0;
    }

    if(m_board->mwService && QLowEnergyService::ServiceDiscovered == s)
    {
        if(++count < m_board->services.size())
        {
            return;
        }

        mbl_mw_settings_set_connection_parameters(m_board, 10.f, 50.f, 0, 1000);

        QLowEnergyCharacteristic mwChar = m_board->mwService->characteristic(METAWARE_CHARACTERISTIC_UUID);
        QLowEnergyDescriptor notificationDesc = mwChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (notificationDesc.isValid())
        {
            m_board->mwService->writeDescriptor(notificationDesc, QByteArray::fromHex("100"));
        }

        MainGui* gui = dynamic_cast<MainGui*>(parent()->parent());
        if(gui)
        {
            connect(gui, SIGNAL(clicked_LED()), this, SLOT(toogleLED()));
            connect(gui, SIGNAL(clicked_FetchData()), this, SLOT(fetchData()));
        }

        mbl_mw_metawearboard_initialize(m_board, is_initialized);
    }
}

void MetaWearDevice::confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    qDebug() << "confirmedDescriptorWrite " << d.name() << " - value: " << value.toHex();
}

void MetaWearDevice::confirmedDescriptorRead(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    qDebug() << "confirmedDescriptorRead " << d.name() << " - value: " << value.toHex();
    if(d.isValid())
    {
        uint8_t* data = (uint8_t*)value.data();
        uint8_t size = value.size();
        mbl_mw_metawearboard_notify_char_changed(m_board,data,size);
    }
}

void MetaWearDevice::readCharacteristic(QLowEnergyCharacteristic gatt,QByteArray value)
{
    qDebug() << "readCharacteristic "  << gatt.uuid().toString() << " - value:" << value.toHex();
    uint8_t* data = (uint8_t*)value.data();
    uint8_t size = value.size();

    MblMwGattChar* mmgc = new MblMwGattChar();
    Uuid2HighLow(gatt.uuid(), mmgc->uuid_high, mmgc->uuid_low);
    mbl_mw_metawearboard_char_read(m_board, mmgc, data, size);
}

void MetaWearDevice::updateCharacteristic(QLowEnergyCharacteristic gatt, QByteArray value)
{
    qDebug() << "updateCharacteristic " << gatt.uuid().toString() << " - value:" << value.toHex();

    if(gatt.uuid() == METAWARE_CHARACTERISTIC_UUID)
    {
        uint8_t* data = (uint8_t*)value.data();
        uint8_t size = value.size();

        mbl_mw_metawearboard_notify_char_changed(m_board, data, size);
    }
}

void MetaWearDevice::toogleLED()
{
    if( mbl_mw_metawearboard_is_initialized(m_board) )
    {
        static bool isOn = true;
        isOn = !isOn;

        MblMwLedPattern pattern;
        mbl_mw_led_load_preset_pattern(&pattern, MBL_MW_LED_PRESET_SOLID);
        mbl_mw_led_write_pattern(m_board, &pattern, MBL_MW_LED_COLOR_GREEN);
        if(isOn)
            mbl_mw_led_stop(m_board);
        else
            mbl_mw_led_play(m_board);
    }
}

static int32_t check_acc_type(MblMwMetaWearBoard* board)
{
    auto acc_type= mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_ACCELEROMETER);
    switch(acc_type)
    {
    case MBL_MW_MODULE_TYPE_NA:
        // should never reach this case statement
        qDebug() << "No accelerometer on this board";
        break;
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
        qDebug() << "Acc Type = MMA8452Q";
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
        qDebug() << "Acc Type = BMI160";
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
        qDebug() << "Acc Type = BMA255";
        break;
    default:
        qDebug() << "Unknown type";
        break;
    }
    return acc_type;
}

void MetaWearDevice::fetchData()
{
    static bool isStarted = false;
    if( !isStarted && mbl_mw_metawearboard_is_initialized(m_board) )
    {
        isStarted = true;
        connect(this,SIGNAL(newData(char,char,QByteArray)),SocketOutput::instance(),SLOT(sendData(char,char,QByteArray)));

        //enable acc sampling
        check_acc_type(m_board);
        auto acc_signal= mbl_mw_acc_get_acceleration_data_signal(m_board);
        mbl_mw_datasignal_subscribe(acc_signal,  acc_handler);
        mbl_mw_acc_enable_acceleration_sampling(m_board);
        mbl_mw_acc_start(m_board);
    }
}

void MetaWearDevice::handle_data(const MblMwMetaWearBoardCustom* board, const MblMwData* data, Signal signal)
{
    QByteArray qdata;
    float value;
    int len = sizeof(float);

    switch(signal)
    {
    case ACCELEROMETER:
        printf("(%.3f, %.3f, %.3f)\n",
               ((MblMwCartesianFloat*)data->value)->x,
               ((MblMwCartesianFloat*)data->value)->y,
               ((MblMwCartesianFloat*)data->value)->z);

        value = ((MblMwCartesianFloat*)data->value)->x;
        qdata.append((char*)&value,len);
        value = ((MblMwCartesianFloat*)data->value)->y;
        qdata.append((char*)&value,len);
        value = ((MblMwCartesianFloat*)data->value)->z;
        qdata.append((char*)&value,len);

        if(board->parent)
            emit board->parent->newData(0x01,msg::ACCELEROMETER,qdata);
        break;
    default:
        break;
    }
}
