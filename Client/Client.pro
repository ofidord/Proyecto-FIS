QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += include/


HEADERS += \
    include/mainwindow.h

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

FORMS += \
    ui/mainwindow.ui

#INCLUDEPATH += E:/Boost/boost-mingw/include/boost-1_67
#LIBS += -LE:/Boost/boost-mingw/lib \
#        -lboost_system-mgw63-mt-d-x32-1_67 \
#        -lws2_32
