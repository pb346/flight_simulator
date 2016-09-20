#-------------------------------------------------
#
# Project created by QtCreator 2016-09-08T13:29:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flight_simulator
TEMPLATE = app

win32:LIBS += -lwinmm

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/joystick_interface.cpp \
    src/state_timer.cpp

HEADERS  += src/mainwindow.h \
    src/joystick_interface.h \
    src/state_timer.h

FORMS    += src/mainwindow.ui

DISTFILES += \
    src/images/adjusted F16.png \
    src/images/Fuel.png \
    src/images/heading.png \
    src/images/sphere.png

RESOURCES += \
    src/images/resource.qrc
