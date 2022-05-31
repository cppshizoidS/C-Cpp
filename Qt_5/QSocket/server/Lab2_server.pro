#-------------------------------------------------
#
# Project created by QtCreator 2011-10-13T18:08:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab2_server
TEMPLATE = app


SOURCES += main.cpp\
    myserver.cpp \
    myclient.cpp \
    dialog.cpp

HEADERS  += \
    myserver.h \
    dialog.h \
    myclient.h

FORMS    += dialog.ui














