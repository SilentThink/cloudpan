QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    protocol.cpp \
    tcpclient.cpp

HEADERS += \
    protocol.h \
    tcpclient.h

FORMS += \
    tcpclient.ui


RESOURCES += \
    config.qrc
