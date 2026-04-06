QT -= gui
QT += testlib

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
           date.cpp

HEADERS += date.h \
           test_date.h
