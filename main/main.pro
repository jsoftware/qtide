
android:{
          CONFIG += mobility
          MOBILITY +=
          TEMPLATE = lib
          TARGET = ../bin/qtide
          DEFINES += "ANDROID" }
else {    TEMPLATE = app
          TARGET = ../bin/jqtx }
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp
win32:config += console
CONFIG+= release

win32-msvc*:QMAKE_LFLAGS += 
win32:RC_FILE = jqt.rc
