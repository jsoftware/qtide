equals(QT_MAJOR_VERSION, 5): QT += widgets
equals(QT_MAJOR_VERSION, 5): DEFINES += QT50

android {
  CONFIG += mobility
  MOBILITY +=
  TEMPLATE = lib
  TARGET = ../bin/qtide
} else {
  TEMPLATE = app
  TARGET = ../bin/jqt
}
win32-msvc*:TARGET = ../bin/jqtx
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp
win32:config += console
CONFIG+= release

win32:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_LFLAGS +=
win32:RC_FILE = jqt.rc
