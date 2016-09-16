#-------------------------------------------------
#
# Project created by QtCreator 2016-09-08T13:29:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flight_simulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

DISTFILES += \
    src/images/adjusted F16.png \
    src/images/Fuel.png \
    src/images/heading.png \
    src/images/sphere.png

RESOURCES += \
    src/images/resource.qrc
