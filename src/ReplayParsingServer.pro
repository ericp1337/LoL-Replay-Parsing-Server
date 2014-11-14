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

unix: LIBS += -L$$PWD/../../qhttp/xbin/ -lqhttp

INCLUDEPATH += $$PWD/../../qhttp/src
DEPENDPATH += $$PWD/../../qhttp/src

unix: PRE_TARGETDEPS += $$PWD/../../qhttp/xbin/libqhttp.a
