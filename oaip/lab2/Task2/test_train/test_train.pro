QT -= gui
QT += core testlib

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += test_train.cpp \
           train.cpp

HEADERS += train.h \
           test_train.h
