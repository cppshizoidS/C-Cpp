win32 {
    QMAKE_CXXFLAGS += -fpermissive
}

QT += core gui network opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = SeaBattle
TEMPLATE = app

win32 {
    LIBS += -lGLU32 -lopengl32 -ljpeg-9
}

unix {
    LIBS += -ljpeg -lGLU
}

SOURCES += main.cpp\
        seabattle.cpp \
    about.cpp \
    connecttoserver.cpp \
    navy.cpp \
    randomlocation.cpp \
    randmaxlocation.cpp \
    solver.cpp \
    edgelocation.cpp \
    chooseuser.cpp \
    shipsettle.cpp \
    shipitem.cpp \
    navypainter.cpp \
    ship.cpp \
    cell.cpp \
    NetLayer.cpp \
    Net.cpp \
    noise.c

HEADERS  += seabattle.h \
    about.h \
    connecttoserver.h \
    navy.h \
    randomlocation.h \
    autolocation.h \
    randmaxlocation.h \
    solver.h \
    ship.h \
    edgelocation.h \
    chooseuser.h \
    shipsettle.h \
    shipitem.h \
    navypainter.h \
    cell.h \
    Net.h \
    noise.h

FORMS    += seabattle.ui \
    about.ui \
    connecttoserver.ui \
    chooseuser.ui \
    shipsettle.ui

RESOURCES += \
    resources.qrc
