TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt
QT += network
QT += widgets
SOURCES += \
        main.cpp \
    proxyserver.cpp

HEADERS += \
    proxyserver.h
