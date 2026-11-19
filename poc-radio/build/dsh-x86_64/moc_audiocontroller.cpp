/****************************************************************************
** Meta object code from reading C++ file 'audiocontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/audiocontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audiocontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AudioController_t {
    QByteArrayData data[14];
    char stringdata0[170];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioController_t qt_meta_stringdata_AudioController = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AudioController"
QT_MOC_LITERAL(1, 16, 19), // "transmittingChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 6), // "active"
QT_MOC_LITERAL(4, 44, 16), // "receivingChanged"
QT_MOC_LITERAL(5, 61, 8), // "logEntry"
QT_MOC_LITERAL(6, 70, 4), // "text"
QT_MOC_LITERAL(7, 75, 17), // "startTransmitting"
QT_MOC_LITERAL(8, 93, 16), // "stopTransmitting"
QT_MOC_LITERAL(9, 110, 14), // "startReceiving"
QT_MOC_LITERAL(10, 125, 13), // "stopReceiving"
QT_MOC_LITERAL(11, 139, 7), // "stopAll"
QT_MOC_LITERAL(12, 147, 12), // "transmitting"
QT_MOC_LITERAL(13, 160, 9) // "receiving"

    },
    "AudioController\0transmittingChanged\0"
    "\0active\0receivingChanged\0logEntry\0"
    "text\0startTransmitting\0stopTransmitting\0"
    "startReceiving\0stopReceiving\0stopAll\0"
    "transmitting\0receiving"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       2,   68, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       5,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   63,    2, 0x0a /* Public */,
       8,    0,   64,    2, 0x0a /* Public */,
       9,    0,   65,    2, 0x0a /* Public */,
      10,    0,   66,    2, 0x0a /* Public */,
      11,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::Bool, 0x00495001,
      13, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void AudioController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioController *_t = static_cast<AudioController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->transmittingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->receivingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->logEntry((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->startTransmitting(); break;
        case 4: _t->stopTransmitting(); break;
        case 5: _t->startReceiving(); break;
        case 6: _t->stopReceiving(); break;
        case 7: _t->stopAll(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AudioController::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioController::transmittingChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AudioController::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioController::receivingChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AudioController::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioController::logEntry)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        AudioController *_t = static_cast<AudioController *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isTransmitting(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isReceiving(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject AudioController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AudioController.data,
      qt_meta_data_AudioController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AudioController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AudioController.stringdata0))
        return static_cast<void*>(const_cast< AudioController*>(this));
    return QObject::qt_metacast(_clname);
}

int AudioController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AudioController::transmittingChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AudioController::receivingChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AudioController::logEntry(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
