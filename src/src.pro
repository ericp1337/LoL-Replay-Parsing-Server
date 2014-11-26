#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T09:20:39
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = ReplayParsingServer
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

PRJDIR = ..
include($$PRJDIR/commondir.pri)

SOURCES += main.cpp \
    handler.cpp \
    handleget.cpp \
    handlepost.cpp

HEADERS += \
    handler.h \
    handleget.h \
    handlepost.h

INSTALLS += target

target.path = /opt/$(TARGET)

unix: LIBS += -L$$PWD/../deps/qhttp/xbin/ -lqhttp

INCLUDEPATH += $$PWD/../deps/qhttp/src
DEPENDPATH += $$PWD/../deps/qhttp/src

unix: PRE_TARGETDEPS += $$PWD/../deps/qhttp/xbin/libqhttp.a
