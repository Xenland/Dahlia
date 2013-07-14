/****************************************************************************
** Meta object code from reading C++ file 'dahlia_client_networking_manager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dahlia_client_networking_manager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dahlia_client_networking_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_dahlia_client_networking_manager_t {
    QByteArrayData data[5];
    char stringdata[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_dahlia_client_networking_manager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_dahlia_client_networking_manager_t qt_meta_stringdata_dahlia_client_networking_manager = {
    {
QT_MOC_LITERAL(0, 0, 32),
QT_MOC_LITERAL(1, 33, 32),
QT_MOC_LITERAL(2, 66, 0),
QT_MOC_LITERAL(3, 67, 17),
QT_MOC_LITERAL(4, 85, 26)
    },
    "dahlia_client_networking_manager\0"
    "new_incoming_command_to_proccess\0\0"
    "incomingDataReady\0proccess_incoming_commands\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dahlia_client_networking_manager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x08,
       4,    0,   31,    2, 0x08,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void dahlia_client_networking_manager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        dahlia_client_networking_manager *_t = static_cast<dahlia_client_networking_manager *>(_o);
        switch (_id) {
        case 0: _t->new_incoming_command_to_proccess(); break;
        case 1: _t->incomingDataReady(); break;
        case 2: _t->proccess_incoming_commands(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (dahlia_client_networking_manager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dahlia_client_networking_manager::new_incoming_command_to_proccess)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject dahlia_client_networking_manager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_dahlia_client_networking_manager.data,
      qt_meta_data_dahlia_client_networking_manager,  qt_static_metacall, 0, 0}
};


const QMetaObject *dahlia_client_networking_manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dahlia_client_networking_manager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dahlia_client_networking_manager.stringdata))
        return static_cast<void*>(const_cast< dahlia_client_networking_manager*>(this));
    return QObject::qt_metacast(_clname);
}

int dahlia_client_networking_manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void dahlia_client_networking_manager::new_incoming_command_to_proccess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
