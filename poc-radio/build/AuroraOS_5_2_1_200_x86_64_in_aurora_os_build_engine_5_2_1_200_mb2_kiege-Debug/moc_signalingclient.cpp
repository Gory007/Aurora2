/****************************************************************************
** Meta object code from reading C++ file 'signalingclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/signalingclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signalingclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SignalingClient_t {
    QByteArrayData data[20];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SignalingClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SignalingClient_t qt_meta_stringdata_SignalingClient = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SignalingClient"
QT_MOC_LITERAL(1, 16, 9), // "connected"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 12), // "disconnected"
QT_MOC_LITERAL(4, 40, 15), // "messageReceived"
QT_MOC_LITERAL(5, 56, 4), // "text"
QT_MOC_LITERAL(6, 61, 13), // "parsedMessage"
QT_MOC_LITERAL(7, 75, 4), // "type"
QT_MOC_LITERAL(8, 80, 4), // "data"
QT_MOC_LITERAL(9, 85, 13), // "errorOccurred"
QT_MOC_LITERAL(10, 99, 9), // "errorText"
QT_MOC_LITERAL(11, 109, 15), // "connectToServer"
QT_MOC_LITERAL(12, 125, 4), // "host"
QT_MOC_LITERAL(13, 130, 4), // "port"
QT_MOC_LITERAL(14, 135, 20), // "disconnectFromServer"
QT_MOC_LITERAL(15, 156, 11), // "sendMessage"
QT_MOC_LITERAL(16, 168, 7), // "message"
QT_MOC_LITERAL(17, 176, 11), // "onReadyRead"
QT_MOC_LITERAL(18, 188, 15), // "onConnectedSlot"
QT_MOC_LITERAL(19, 204, 18) // "onDisconnectedSlot"

    },
    "SignalingClient\0connected\0\0disconnected\0"
    "messageReceived\0text\0parsedMessage\0"
    "type\0data\0errorOccurred\0errorText\0"
    "connectToServer\0host\0port\0"
    "disconnectFromServer\0sendMessage\0"
    "message\0onReadyRead\0onConnectedSlot\0"
    "onDisconnectedSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SignalingClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    1,   71,    2, 0x06 /* Public */,
       6,    2,   74,    2, 0x06 /* Public */,
       9,    1,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    2,   82,    2, 0x0a /* Public */,
      14,    0,   87,    2, 0x0a /* Public */,
      15,    1,   88,    2, 0x0a /* Public */,
      17,    0,   91,    2, 0x08 /* Private */,
      18,    0,   92,    2, 0x08 /* Private */,
      19,    0,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::QString,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,   12,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SignalingClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SignalingClient *_t = static_cast<SignalingClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->parsedMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->connectToServer((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 6: _t->disconnectFromServer(); break;
        case 7: _t->sendMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onReadyRead(); break;
        case 9: _t->onConnectedSlot(); break;
        case 10: _t->onDisconnectedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SignalingClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SignalingClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SignalingClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SignalingClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SignalingClient::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SignalingClient::messageReceived)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SignalingClient::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SignalingClient::parsedMessage)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SignalingClient::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SignalingClient::errorOccurred)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject SignalingClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SignalingClient.data,
      qt_meta_data_SignalingClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SignalingClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SignalingClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SignalingClient.stringdata0))
        return static_cast<void*>(const_cast< SignalingClient*>(this));
    return QObject::qt_metacast(_clname);
}

int SignalingClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void SignalingClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void SignalingClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void SignalingClient::messageReceived(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SignalingClient::parsedMessage(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SignalingClient::errorOccurred(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
