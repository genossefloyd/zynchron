/****************************************************************************
** Meta object code from reading C++ file 'bluetoothconnector.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../input/bluetoothconnector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bluetoothconnector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BluetoothConnector_t {
    QByteArrayData data[20];
    char stringdata0[328];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BluetoothConnector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BluetoothConnector_t qt_meta_stringdata_BluetoothConnector = {
    {
QT_MOC_LITERAL(0, 0, 18), // "BluetoothConnector"
QT_MOC_LITERAL(1, 19, 7), // "updated"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 16), // "deviceDiscovered"
QT_MOC_LITERAL(4, 45, 20), // "QBluetoothDeviceInfo"
QT_MOC_LITERAL(5, 66, 6), // "device"
QT_MOC_LITERAL(6, 73, 15), // "deviceScanError"
QT_MOC_LITERAL(7, 89, 37), // "QBluetoothDeviceDiscoveryAgen..."
QT_MOC_LITERAL(8, 127, 5), // "error"
QT_MOC_LITERAL(9, 133, 12), // "scanFinished"
QT_MOC_LITERAL(10, 146, 15), // "deviceConnected"
QT_MOC_LITERAL(11, 162, 18), // "deviceDisconnected"
QT_MOC_LITERAL(12, 181, 17), // "serviceDiscovered"
QT_MOC_LITERAL(13, 199, 14), // "QBluetoothUuid"
QT_MOC_LITERAL(14, 214, 15), // "serviceScanDone"
QT_MOC_LITERAL(15, 230, 15), // "controllerError"
QT_MOC_LITERAL(16, 246, 27), // "QLowEnergyController::Error"
QT_MOC_LITERAL(17, 274, 19), // "serviceStateChanged"
QT_MOC_LITERAL(18, 294, 31), // "QLowEnergyService::ServiceState"
QT_MOC_LITERAL(19, 326, 1) // "s"

    },
    "BluetoothConnector\0updated\0\0"
    "deviceDiscovered\0QBluetoothDeviceInfo\0"
    "device\0deviceScanError\0"
    "QBluetoothDeviceDiscoveryAgent::Error\0"
    "error\0scanFinished\0deviceConnected\0"
    "deviceDisconnected\0serviceDiscovered\0"
    "QBluetoothUuid\0serviceScanDone\0"
    "controllerError\0QLowEnergyController::Error\0"
    "serviceStateChanged\0QLowEnergyService::ServiceState\0"
    "s"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BluetoothConnector[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   65,    2, 0x08 /* Private */,
       6,    1,   68,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,
      12,    1,   74,    2, 0x08 /* Private */,
      14,    0,   77,    2, 0x08 /* Private */,
      15,    1,   78,    2, 0x08 /* Private */,
      17,    1,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,    2,
    QMetaType::Void, 0x80000000 | 18,   19,

       0        // eod
};

void BluetoothConnector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BluetoothConnector *_t = static_cast<BluetoothConnector *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        case 1: _t->deviceDiscovered((*reinterpret_cast< const QBluetoothDeviceInfo(*)>(_a[1]))); break;
        case 2: _t->deviceScanError((*reinterpret_cast< QBluetoothDeviceDiscoveryAgent::Error(*)>(_a[1]))); break;
        case 3: _t->scanFinished(); break;
        case 4: _t->deviceConnected(); break;
        case 5: _t->deviceDisconnected(); break;
        case 6: _t->serviceDiscovered((*reinterpret_cast< const QBluetoothUuid(*)>(_a[1]))); break;
        case 7: _t->serviceScanDone(); break;
        case 8: _t->controllerError((*reinterpret_cast< QLowEnergyController::Error(*)>(_a[1]))); break;
        case 9: _t->serviceStateChanged((*reinterpret_cast< QLowEnergyService::ServiceState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothDeviceInfo >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothUuid >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyController::Error >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyService::ServiceState >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BluetoothConnector::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BluetoothConnector::updated)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject BluetoothConnector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BluetoothConnector.data,
      qt_meta_data_BluetoothConnector,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BluetoothConnector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BluetoothConnector::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BluetoothConnector.stringdata0))
        return static_cast<void*>(const_cast< BluetoothConnector*>(this));
    return QObject::qt_metacast(_clname);
}

int BluetoothConnector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void BluetoothConnector::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
