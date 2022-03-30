QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Exception.cpp \
    Figure.cpp \
    figureitem.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Exception.h \
    Figure.h \
    figureitem.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/circle.png \
    images/cylinder.png \
    images/delete.png \
    images/exit.png \
    images/ring.png \
    images/triangle.png

RESOURCES += \
    images/
