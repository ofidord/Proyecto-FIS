QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
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
