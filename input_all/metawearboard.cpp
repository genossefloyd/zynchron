#include "metawearboard.h"
#include "peripheral/led.h"
#include "core/status.h"
#include "core/settings.h"

#include <sstream>
#include <string>
#include <stdio.h>
#include <inttypes.h>

#include <maingui.h>

const QBluetoothUuid metaweardevice::METAWARE_SERVICE_UUID = HighLow2Uuid(METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_high,
                                                    METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_low);
const QBluetoothUuid metaweardevice::METAWARE_CHARACTERISTIC_UUID = HighLow2Uuid(METAWEAR_SERVICE_NOTIFY_CHAR.uuid_high,
                                                    METAWEAR_SERVICE_NOTIFY_CHAR.uuid_low);

ServiceMap metaweardevice::m_services;

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

void metaweardevice::write_gatt_char(const void* /*caller*/, const MblMwGattChar *characteristic,
        const uint8_t *value, uint8_t length)
{
    QBluetoothUuid uuidService = HighLow2Uuid(characteristic->service_uuid_high,characteristic->service_uuid_low);
    QBluetoothUuid uuidChar = HighLow2Uuid(characteristic->uuid_high,characteristic->uuid_low);

    ServiceMap::iterator itService = m_services.find(uuidService);
    if(m_services.end() != itService)
    {
        QLowEnergyCharacteristic qtCharacterstic = m_services[uuidService]->characteristic(uuidChar);
        QByteArray newValue((const char*)value,length);

        qDebug() << "Writting to GATT " << qtCharacterstic.uuid().toString() << " - value: " << newValue.toHex();
        itService.value()->writeCharacteristic(qtCharacterstic,newValue);
    }
}

void metaweardevice::read_gatt_char(const void* /*caller*/, const MblMwGattChar *characteristic)
{
    QBluetoothUuid uuidService = HighLow2Uuid(characteristic->service_uuid_high,characteristic->service_uuid_low);
    QBluetoothUuid uuidChar = HighLow2Uuid(characteristic->uuid_high,characteristic->uuid_low);

    ServiceMap::iterator itService = m_services.find(uuidService);
    if(m_services.end() != itService)
    {
        QLowEnergyCharacteristic qtCharacterstic = itService.value()->characteristic(uuidChar);
        if(qtCharacterstic.isValid())
        {
            itService.value()->readCharacteristic(qtCharacterstic);
        }
    }
}

void metaweardevice::cb_IsInitialized(MblMwMetaWearBoard* board, int32_t status)
{
    qDebug() << "Board status: " << status;
}

metaweardevice::metaweardevice(QObject *parent)
    : QObject(parent)
    , m_board(NULL)
    , m_control(NULL)
    , m_mwService(NULL)
{
    m_connection = { write_gatt_char, read_gatt_char };
}

metaweardevice::~metaweardevice()
{
    cleanup();
}

void metaweardevice::cleanup()
{
    if( m_board )
    {
        mbl_mw_metawearboard_tear_down(m_board);
        mbl_mw_metawearboard_free(m_board);
        m_board = 0;
    }

    if(m_mwService && m_notificationDesc.isValid())
    {
        m_mwService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0000"));
    }
    qDeleteAll(m_services);
    m_services.clear();

    if (m_control)
    {
        m_control->disconnectFromDevice();
        delete m_control;
        m_control = 0;
    }
}

bool metaweardevice::init(QLowEnergyController *controller )
{
    cleanup();

    m_control = controller;

    connect(m_control, SIGNAL(connected()),
            this, SLOT(deviceConnected()));
    connect(m_control, SIGNAL(disconnected()),
            this, SLOT(deviceDisconnected()));

    connect(m_control, SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this, SLOT(serviceDiscovered(QBluetoothUuid)));
    connect(m_control, SIGNAL(discoveryFinished()),
            this, SLOT(serviceScanDone()));

    connect(m_control, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(controllerError(QLowEnergyController::Error)));

    m_control->connectToDevice();

    return true;
}

void metaweardevice::deviceConnected()
{
    m_control->discoverServices();
}

void metaweardevice::deviceDisconnected()
{
    qWarning() << "Remote device disconnected";
}

void metaweardevice::controllerError(QLowEnergyController::Error error)
{
    qDebug() << "Cannot connect to remote device.";
    qWarning() << "Controller Error:" << error;
}

void metaweardevice::serviceDiscovered(const QBluetoothUuid &gatt)
{
    if( !m_services.contains(gatt) )
    {
        qDebug() << "New service discovered: " << gatt.toString();

        QLowEnergyService* service = m_control->createServiceObject(gatt, this);

        connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));

        connect(service, SIGNAL(characteristicRead(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(readCharacteristic(QLowEnergyCharacteristic,QByteArray)));
        connect(service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(updateCharacteristic(QLowEnergyCharacteristic,QByteArray)));

        m_services[gatt] = service;

        if( gatt == METAWARE_SERVICE_UUID)
        {
            m_mwService = service;

            connect(service, SIGNAL(descriptorRead(QLowEnergyDescriptor,QByteArray)),
                    this, SLOT(confirmedDescriptorRead(QLowEnergyDescriptor,QByteArray)));
            connect(service, SIGNAL(descriptorWritten(QLowEnergyDescriptor,QByteArray)),
                    this, SLOT(confirmedDescriptorWrite(QLowEnergyDescriptor,QByteArray)));
        }
    }
}

void metaweardevice::serviceScanDone()
{
    qDebug() << "Service scan done ";

    ServiceMap::iterator it = m_services.begin();
    while(it != m_services.end())
    {
        it.value()->discoverDetails();
        ++it;
    }
}

void metaweardevice::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Service State changed to: "<< s ;
    static int count = 0;

    if(m_mwService && QLowEnergyService::ServiceDiscovered == s)
    {
        if(++count < m_services.size())
        {
            return;
        }

        m_board = mbl_mw_metawearboard_create(&m_connection);
        mbl_mw_settings_set_connection_parameters(m_board, 10.f, 500.f, 0, 6000);

        QLowEnergyCharacteristic mwChar = m_mwService->characteristic(METAWARE_CHARACTERISTIC_UUID);
        m_notificationDesc = mwChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (m_notificationDesc.isValid())
        {
            m_mwService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("100"));
        }

        MainGui* gui = dynamic_cast<MainGui*>(parent()->parent());
        if(gui)
        {
            connect(gui, SIGNAL(clicked_LED()), this, SLOT(setLED()));
        }
    }
}

void metaweardevice::confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    qDebug() << "confirmedDescriptorWrite " << d.name() << " - value: " << value.toHex();
}

void metaweardevice::confirmedDescriptorRead(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    qDebug() << "confirmedDescriptorRead " << d.name() << " - value: " << value.toHex();
    if(d.isValid())
    {
        uint8_t* data = (uint8_t*)value.data();
        uint8_t size = value.size();
        mbl_mw_metawearboard_notify_char_changed(m_board,data,size);
    }
}

void metaweardevice::readCharacteristic(QLowEnergyCharacteristic gatt,QByteArray value)
{
    qDebug() << "readCharacteristic "  << gatt.uuid().toString() << " - value:" << value.toHex();
    uint8_t* data = (uint8_t*)value.data();
    uint8_t size = value.size();

    MblMwGattChar* mmgc = new MblMwGattChar();
    Uuid2HighLow(gatt.uuid(), mmgc->uuid_high, mmgc->uuid_low);
    mbl_mw_metawearboard_char_read(m_board, mmgc, data, size);
}

void metaweardevice::updateCharacteristic(QLowEnergyCharacteristic gatt, QByteArray value)
{
    qDebug() << "updateCharacteristic " << gatt.uuid().toString() << " - value:" << value.toHex();

    if(gatt.uuid() == METAWARE_CHARACTERISTIC_UUID)
    {
        uint8_t* data = (uint8_t*)value.data();
        uint8_t size = value.size();

        mbl_mw_metawearboard_notify_char_changed(m_board, data, size);
    }
}

void metaweardevice::setLED()
{
    bool isInit = mbl_mw_metawearboard_is_initialized(m_board);
    qDebug() << "Board is initialised: " << isInit;
    if(m_board)
    {
        if(isInit)
        {
            MblMwLedPattern pattern;
            // Load the blink pattern
            mbl_mw_led_load_preset_pattern(&pattern, MBL_MW_LED_PRESET_BLINK);
            // Write the blink pattern to the blue channel
            mbl_mw_led_write_pattern(m_board, &pattern, MBL_MW_LED_COLOR_BLUE);
            mbl_mw_led_play(m_board);
        }
        else
        {
            mbl_mw_metawearboard_initialize(m_board, cb_IsInitialized);
        }
    }
}
