#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T21:40:58
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mainwindow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fonts.cpp \
    goto.cpp

HEADERS  += mainwindow.h \
    fonts.h \
    goto.h

RESOURCES += \
    images.qrc
