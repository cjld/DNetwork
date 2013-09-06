#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T23:28:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DNetwork
TEMPLATE = app

LIBS += -lwsock32

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        widget.cpp \
    dnetwork.cpp

HEADERS  += widget.h \
    dnetwork.h

FORMS    += widget.ui
