
greaterThan(QT_VERSION,4.7.0): DEFINES += QT47
greaterThan(QT_VERSION,4.8.0): DEFINES += QT48
equals(QT_MAJOR_VERSION, 5): DEFINES += QT50
!lessThan(QT_VERSION,5.3.0): DEFINES += QT53

android {
  CONFIG += mobility
  MOBILITY +=
  DEFINES += QT_OS_ANDROID
  TEMPLATE = lib
  TARGET = qtide
  !contains(DEFINES,QT50): error(requires Qt5)
} else {
  TEMPLATE = app
  TARGET = jqt
}

contains(DEFINES,QT50): QT += widgets

CONFIG(debug, debug|release) {
  rel = debug
} else {
  rel = release
}

linux-g++: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32: QMAKE_TARGET.arch = x86
linux-g++-64: QMAKE_TARGET.arch = x86_64
linux-cross: QMAKE_TARGET.arch = x86
win32-cross-32: QMAKE_TARGET.arch = x86
win32-cross: QMAKE_TARGET.arch = x86_64
win32-g++: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
win32-msvc*: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
android: QMAKE_TARGET.arch = arm
linux-raspi: QMAKE_TARGET.arch = arm

equals(QMAKE_TARGET.arch , i686): QMAKE_TARGET.arch = x86

win32: arch = win-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch
macx: arch = mac-$$QMAKE_TARGET.arch
unix:!macx: arch = linux-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch

BUILDROOT = build/$$arch/$$rel
TARGETROOT = ../bin/$$arch/$$rel
DESTDIR = $$TARGETROOT
DLLDESTDIR = $$TARGETROOT

OBJECTS_DIR = $$BUILDROOT/obj
MOC_DIR = $$BUILDROOT/moc
RCC_DIR = $$BUILDROOT/rcc
UI_DIR = $$BUILDROOT/ui

linux-raspi: DEFINES += RASPI

win32-msvc*:TARGET = ../bin/jqt
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp jepath.cpp
win32:config += console
CONFIG+= release

win32:!win32-msvc*:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_LFLAGS +=
win32:RC_FILE = jqt.rc
