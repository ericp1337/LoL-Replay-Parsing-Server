#-------------------------------------------------
#
# Project created by QtCreator 2014-11-25T17:31:21
#
#-------------------------------------------------

QT       += core gui network quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    settings.h

FORMS    += mainwindow.ui \
    settings.ui

PRJDIR = ../..
include($$PRJDIR/commondir.pri)

RESOURCES += \
    client.qrc

DISTFILES +=
