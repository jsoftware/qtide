
greaterThan(QT_VERSION,4.7.0): DEFINES += QT47
greaterThan(QT_VERSION,4.8.0): DEFINES += QT48
equals(QT_MAJOR_VERSION, 5): DEFINES += QT50
!lessThan(QT_VERSION,5.3.0): DEFINES += QT53
!lessThan(QT_VERSION,5.4.0): DEFINES += QT54

TEMPLATE = app
TARGET = jqt

# contains(DEFINES,QT50): QT += widgets

CONFIG(debug, debug|release) {
  rel = debug
} else {
  rel = release
}

# export JQTOLECOM before qmake
JQTOLECOM = $$(JQTOLECOM)
win32: !isEmpty(JQTOLECOM) {
  message(building ole server jqt)
  DEFINES += JQTOLECOM
}

linux-g++: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32: QMAKE_TARGET.arch = x86
linux-g++-64: QMAKE_TARGET.arch = x86_64
linux-cross: QMAKE_TARGET.arch = x86
win32-cross-32: QMAKE_TARGET.arch = x86
win32-cross: QMAKE_TARGET.arch = x86_64
win32-g++: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
win32-msvc*: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-raspi: QMAKE_TARGET.arch = arm

equals(QMAKE_TARGET.arch , i686): QMAKE_TARGET.arch = x86
ABI=$$(ABI)

win32: arch = win-$$QMAKE_TARGET.arch
macx: arch = mac-$$QMAKE_TARGET.arch
unix:!macx: arch = linux-$$QMAKE_TARGET.arch

BUILDROOT = build/$$arch/$$rel
TARGETROOT = ../bin/$$arch/$$rel
DESTDIR = $$TARGETROOT
DLLDESTDIR = $$TARGETROOT

OBJECTS_DIR = $$BUILDROOT/obj
MOC_DIR = $$BUILDROOT/moc
RCC_DIR = $$BUILDROOT/rcc
UI_DIR = $$BUILDROOT/ui

linux-raspi: DEFINES += RASPI
macx:CONFIG += c++11

win32-msvc*:TARGET = ../bin/jqt
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp jepath.cpp
contains(DEFINES,JQTOLECOM) {
  win32:OBJECTS += $$arch/$$rel/obj/jdllcomx.obj
}
# win32:config += console
CONFIG+= release

win32:LIBS += -lole32 -loleaut32 -luuid -ladvapi32
win32-msvc*:QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
win32-msvc*:QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -Zc:strictStrings
win32:!win32-msvc*:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_LFLAGS +=
macx:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
win32:RC_FILE = jqt.rc
