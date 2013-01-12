
android:{
          CONFIG += mobility
          MOBILITY +=
          TEMPLATE = lib
          TARGET = ../bin/qtide
          DEFINES += "ANDROID" }
else {    TEMPLATE = app
          TARGET = ../bin/jqt }
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp
win32:config += console
unix:CONFIG+= release
android:CONFIG+= release

win32:QMAKE_LFLAGS += -static-libgcc
