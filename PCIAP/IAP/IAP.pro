#-------------------------------------------------
#
# Project created by QtCreator 2012-05-11T16:19:11
#
#-------------------------------------------------

QT       += core gui

TARGET = IAP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    win_qextserialport.cpp \
    qextserialbase.cpp \
    dialogsetcom.cpp \
    sysdef.cpp

HEADERS  += mainwindow.h \
    qextserialbase.h \
    win_qextserialport.h \
    dialogsetcom.h \
    sysdef.h

FORMS    += mainwindow.ui \
    dialogsetcom.ui
