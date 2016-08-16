/****************************************************************************
** Meta object code from reading C++ file 'metawearboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../input/metawearboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'metawearboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_metawearboard_t {
    QByteArrayData data[21];
    char stringdata0[362];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_metawearboard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_metawearboard_t qt_meta_stringdata_metawearboard = {
    {
QT_MOC_LITERAL(0, 0, 13), // "metawearboard"
QT_MOC_LITERAL(1, 14, 15), // "deviceConnected"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 18), // "deviceDisconnected"
QT_MOC_LITERAL(4, 50, 15), // "controllerError"
QT_MOC_LITERAL(5, 66, 27), // "QLowEnergyController::Error"
QT_MOC_LITERAL(6, 94, 17), // "serviceDiscovered"
QT_MOC_LITERAL(7, 112, 14), // "QBluetoothUuid"
QT_MOC_LITERAL(8, 127, 15), // "serviceScanDone"
QT_MOC_LITERAL(9, 143, 19), // "serviceStateChanged"
QT_MOC_LITERAL(10, 163, 31), // "QLowEnergyService::ServiceState"
QT_MOC_LITERAL(11, 195, 1), // "s"
QT_MOC_LITERAL(12, 197, 20), // "updateCharacteristic"
QT_MOC_LITERAL(13, 218, 24), // "QLowEnergyCharacteristic"
QT_MOC_LITERAL(14, 243, 18), // "readCharacteristic"
QT_MOC_LITERAL(15, 262, 21), // "writtenCharacteristic"
QT_MOC_LITERAL(16, 284, 24), // "confirmedDescriptorWrite"
QT_MOC_LITERAL(17, 309, 20), // "QLowEnergyDescriptor"
QT_MOC_LITERAL(18, 330, 1), // "d"
QT_MOC_LITERAL(19, 332, 5), // "value"
QT_MOC_LITERAL(20, 338, 23) // "confirmedDescriptorRead"

    },
    "metawearboard\0deviceConnected\0\0"
    "deviceDisconnected\0controllerError\0"
    "QLowEnergyController::Error\0"
    "serviceDiscovered\0QBluetoothUuid\0"
    "serviceScanDone\0serviceStateChanged\0"
    "QLowEnergyService::ServiceState\0s\0"
    "updateCharacteristic\0QLowEnergyCharacteristic\0"
    "readCharacteristic\0writtenCharacteristic\0"
    "confirmedDescriptorWrite\0QLowEnergyDescriptor\0"
    "d\0value\0confirmedDescriptorRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_metawearboard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    1,   71,    2, 0x08 /* Private */,
       6,    1,   74,    2, 0x08 /* Private */,
       8,    0,   77,    2, 0x08 /* Private */,
       9,    1,   78,    2, 0x08 /* Private */,
      12,    2,   81,    2, 0x08 /* Private */,
      14,    2,   86,    2, 0x08 /* Private */,
      15,    2,   91,    2, 0x08 /* Private */,
      16,    2,   96,    2, 0x08 /* Private */,
      20,    2,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13, QMetaType::QByteArray,    2,    2,
    QMetaType::Void, 0x80000000 | 13, QMetaType::QByteArray,    2,    2,
    QMetaType::Void, 0x80000000 | 13, QMetaType::QByteArray,    2,    2,
    QMetaType::Void, 0x80000000 | 17, QMetaType::QByteArray,   18,   19,
    QMetaType::Void, 0x80000000 | 17, QMetaType::QByteArray,   18,   19,

       0        // eod
};

void metawearboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        metawearboard *_t = static_cast<metawearboard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceConnected(); break;
        case 1: _t->deviceDisconnected(); break;
        case 2: _t->controllerError((*reinterpret_cast< QLowEnergyController::Error(*)>(_a[1]))); break;
        case 3: _t->serviceDiscovered((*reinterpret_cast< const QBluetoothUuid(*)>(_a[1]))); break;
        case 4: _t->serviceScanDone(); break;
        case 5: _t->serviceStateChanged((*reinterpret_cast< QLowEnergyService::ServiceState(*)>(_a[1]))); break;
        case 6: _t->updateCharacteristic((*reinterpret_cast< QLowEnergyCharacteristic(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 7: _t->readCharacteristic((*reinterpret_cast< QLowEnergyCharacteristic(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 8: _t->writtenCharacteristic((*reinterpret_cast< QLowEnergyCharacteristic(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 9: _t->confirmedDescriptorWrite((*reinterpret_cast< const QLowEnergyDescriptor(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 10: _t->confirmedDescriptorRead((*reinterpret_cast< const QLowEnergyDescriptor(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyController::Error >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothUuid >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLowEnergyService::ServiceState >(); break;
            }
            break;
        }
    }
}

const QMetaObject metawearboard::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_metawearboard.data,
      qt_meta_data_metawearboard,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *metawearboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *metawearboard::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_metawearboard.stringdata0))
        return static_cast<void*>(const_cast< metawearboard*>(this));
    return QObject::qt_metacast(_clname);
}

int metawearboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
