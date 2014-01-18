#-------------------------------------------------
#
# Project created by QtCreator 2013-11-29T22:37:04
#
#-------------------------------------------------

QT       += network
QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuickMsg
TEMPLATE = app

#############for release######################
CONFIG += release
#############for debug########################
#CONFIG +=debug



SOURCES += main.cpp\
        mainwindow.cpp \
    tshifr.cpp

HEADERS  += mainwindow.h \
    tshifr.h

FORMS    += mainwindow.ui
