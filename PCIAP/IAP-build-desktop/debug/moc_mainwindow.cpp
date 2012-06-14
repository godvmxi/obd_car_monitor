/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Jun 6 11:08:49 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IAP/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   12,   11,   11, 0x0a,
      57,   11,   11,   11, 0x08,
      84,   11,   11,   11, 0x08,
     112,   11,   11,   11, 0x08,
     145,   11,   11,   11, 0x08,
     175,   11,   11,   11, 0x08,
     201,   11,   11,   11, 0x08,
     237,   11,   11,   11, 0x08,
     270,   11,   11,   11, 0x08,
     294,   11,   11,   11, 0x08,
     318,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0setting,com\0"
    "getComInfo(PortSettings,QString)\0"
    "on_action_exit_triggered()\0"
    "on_pushButtonSend_clicked()\0"
    "on_action_disconnect_triggered()\0"
    "on_action_connect_triggered()\0"
    "on_serial_set_triggered()\0"
    "on_spinBoxBufSize_valueChanged(int)\0"
    "on_checkBoxHex_stateChanged(int)\0"
    "on_open_bin_triggered()\0on_open_hex_triggered()\0"
    "readReady()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: getComInfo((*reinterpret_cast< PortSettings(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: on_action_exit_triggered(); break;
        case 2: on_pushButtonSend_clicked(); break;
        case 3: on_action_disconnect_triggered(); break;
        case 4: on_action_connect_triggered(); break;
        case 5: on_serial_set_triggered(); break;
        case 6: on_spinBoxBufSize_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: on_checkBoxHex_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: on_open_bin_triggered(); break;
        case 9: on_open_hex_triggered(); break;
        case 10: readReady(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
