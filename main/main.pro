android {
  CONFIG += mobility
  MOBILITY +=
  TEMPLATE = lib
  TARGET = qtide
  !equals(QT_MAJOR_VERSION, 5): error(requires Qt5)
} else {
  TEMPLATE = app
  TARGET = jqt
}

equals(QT_MAJOR_VERSION, 5): QT += widgets
equals(QT_MAJOR_VERSION, 5): DEFINES += QT50

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
android: QMAKE_TARGET.arch = arm
linux-raspi: QMAKE_TARGET.arch = arm

win32: arch = win-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch
macx: arch = mac-$$QMAKE_TARGET.arch
unix:!macx: arch = linux-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch
linux-raspi:arch = raspi-$$QMAKE_TARGET.arch

BUILDROOT = build/$$arch/$$rel
TARGETROOT = ../bin/$$arch/$$rel
DESTDIR = $$TARGETROOT
DLLDESTDIR = $$TARGETROOT

OBJECTS_DIR = $$BUILDROOT/obj
MOC_DIR = $$BUILDROOT/moc
RCC_DIR = $$BUILDROOT/rcc
UI_DIR = $$BUILDROOT/ui

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
