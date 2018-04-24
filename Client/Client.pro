QT -= gui
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle


TARGET = Client


DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += include/


HEADERS +=

SOURCES += \
    src/main.cpp

FORMS +=

#INCLUDEPATH += E:/Boost/boost-mingw/include/boost-1_67
#LIBS += -LE:/Boost/boost-mingw/lib \
#        -lboost_system-mgw63-mt-d-x32-1_67 \
#        -lws2_32
