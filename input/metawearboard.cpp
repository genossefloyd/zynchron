#include "metawearboard.h"
#include "peripheral/led.h"
#include "core/status.h"

#include <sstream>
#include <string>
#include <stdio.h>

const QBluetoothUuid metawearboard::METAWARE_SERVICE_UUID = HighLow2Uuid(METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_high,
                                                    METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_low);
const QBluetoothUuid metawearboard::METAWARE_CHARACTERISTIC_UUID = HighLow2Uuid(METAWEAR_SERVICE_NOTIFY_CHAR.uuid_high,
                                                    METAWEAR_SERVICE_NOTIFY_CHAR.uuid_low);

ServiceMap metawearboard::m_services;

QUuid HighLow2Uuid(const uint64_t high, const uint64_t low)
{
    std::stringstream sstream;
    sstream << std::hex << high;
    sstream << std::hex << low;

    std::string s = sstream.str();

    unsigned long p0;
    int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

    sscanf(s.c_str(), "%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
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

void metawearboard::write_gatt_char(const void* caller, const MblMwGattChar *characteristic,
        const uint8_t *value, uint8_t length)
{
    QBluetoothUuid uuidService = HighLow2Uuid(characteristic->service_uuid_high,characteristic->service_uuid_low);
    QBluetoothUuid uuidChar = HighLow2Uuid(characteristic->uuid_high,characteristic->uuid_low);

    ServiceMap::iterator itService = m_services.find(uuidService);
    if(m_services.end() != itService)
    {
        QLowEnergyCharacteristic qtCharacterstic = m_services[uuidService]->characteristic(uuidChar);
        QByteArray newValue((const char*)value,length);
        itService.value()->writeCharacteristic(qtCharacterstic,newValue);
    }
}

void metawearboard::read_gatt_char(const void* caller, const MblMwGattChar *characteristic)
{
    QBluetoothUuid uuidService = HighLow2Uuid(characteristic->service_uuid_high,characteristic->service_uuid_low);
    QBluetoothUuid uuidChar = HighLow2Uuid(characteristic->uuid_high,characteristic->uuid_low);

    ServiceMap::iterator itService = m_services.find(uuidService);
    if(m_services.end() != itService)
    {
        QLowEnergyCharacteristic qtCharacterstic = itService.value()->characteristic(uuidChar);
        itService.value()->readCharacteristic(qtCharacterstic);
    }
}

void metawearboard::cb_IsInitialized(MblMwMetaWearBoard *board, int32_t status)
{
    qDebug() << "Board status: " << status;
}

metawearboard::metawearboard(QObject *parent)
    : QObject(parent)
    , m_board(NULL)
    , m_control(NULL)
    , m_mwService(NULL)
{
    m_connection = { write_gatt_char, read_gatt_char };
}

metawearboard::~metawearboard()
{
    cleanup();
}

void metawearboard::cleanup()
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

bool metawearboard::init(QLowEnergyController *controller )
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

void metawearboard::deviceConnected()
{
    m_control->discoverServices();
}

void metawearboard::deviceDisconnected()
{
    qWarning() << "Remote device disconnected";
}

void metawearboard::controllerError(QLowEnergyController::Error error)
{
    qDebug() << "Cannot connect to remote device.";
    qWarning() << "Controller Error:" << error;
}

void metawearboard::serviceDiscovered(const QBluetoothUuid &gatt)
{
    qDebug() << "Service discovered: " << gatt.toString() << ". Waiting for service scan to be done...";

    if( !m_services.contains(gatt) )
    {
        QLowEnergyService* service = m_control->createServiceObject(gatt, this);

        connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));

        connect(service, SIGNAL(characteristicRead(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(readCharacteristic(QLowEnergyCharacteristic,QByteArray)));
        connect(service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(updateCharacteristic(QLowEnergyCharacteristic,QByteArray)));
        connect(service, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)),
                    this, SLOT(writtenCharacteristic(QLowEnergyCharacteristic,QByteArray)));

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

void metawearboard::serviceScanDone()
{
    //m_service->discoverDetails();
    if(m_mwService)
    {
        const QLowEnergyCharacteristic btleChar = m_mwService->characteristic(METAWARE_CHARACTERISTIC_UUID);
        if (!btleChar.isValid())
        {
            qDebug() << "Data is not valid";
            return;
        }

        m_board = mbl_mw_metawearboard_create(&m_connection);
        mbl_mw_metawearboard_initialize(m_board, cb_IsInitialized);

        m_notificationDesc = btleChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (m_notificationDesc.isValid())
        {
            QByteArray t = QByteArray::fromHex("10");
            m_mwService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
            qDebug() << "Measuring";
        }
    }
}

void metawearboard::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Service State changed to: "<< s ;
}

void metawearboard::confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    if (d.isValid() && d == m_notificationDesc && value == QByteArray("0000"))
    {
        //disabled notifications -> assume disconnect intent
        //m_control->disconnectFromDevice();
        //delete m_mwService;
        //m_service = 0;
    }
}

void metawearboard::confirmedDescriptorRead(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    qDebug() << "confirmedDescriptorRead";
    if(d.isValid())
    {
        uint8_t* data = (uint8_t*)value.data();
        uint8_t size = value.size();
        mbl_mw_metawearboard_notify_char_changed(m_board,data,size);
    }
}

void metawearboard::readCharacteristic(QLowEnergyCharacteristic qtCharacterstic,QByteArray value)
{
    qDebug() << "readCharacteristic";
    uint8_t* data = (uint8_t*)value.data();
    uint8_t size = value.size();

    MblMwGattChar* mmgc = new MblMwGattChar();
    Uuid2HighLow(qtCharacterstic.uuid(), mmgc->uuid_high, mmgc->uuid_low);
    mbl_mw_connection_char_read(m_board, mmgc, data, size);
}

void metawearboard::writtenCharacteristic(QLowEnergyCharacteristic qtCharacterstic, QByteArray value)
{
    qDebug() << "writtenCharacteristic";
}

void metawearboard::updateCharacteristic(QLowEnergyCharacteristic qtCharacterstic, QByteArray value)
{
    qDebug() << "updateCharacteristic";
}
