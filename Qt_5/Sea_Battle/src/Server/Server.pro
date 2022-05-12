
QT       += core

QT       -= gui

LIBS += -lpthread

win32 {
    LIBS += -lws2_32
}

TARGET = SeaBattleServer
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app



SOURCES += main.cpp

HEADERS += \
    logger.h
