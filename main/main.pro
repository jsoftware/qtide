
JDLLVER = 8.06    # ignored if not FHS

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

linux-g++: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32: QMAKE_TARGET.arch = x86
linux-g++-64: QMAKE_TARGET.arch = x86_64
linux-cross: QMAKE_TARGET.arch = x86
win32-cross-32: QMAKE_TARGET.arch = x86
win32-cross: QMAKE_TARGET.arch = x86_64
win32-g++: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
win32-msvc*: QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-raspi: QMAKE_TARGET.arch = armv6l
linux-armv6l: QMAKE_TARGET.arch = armv6l

equals(QMAKE_TARGET.arch , i686): QMAKE_TARGET.arch = x86
ABI=$$(ABI)

equals(QMAKE_TARGET.arch , armv6l): {
  message(building raspberry pi jqt)
  DEFINES += RASPI
}

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

DEFINES += JDLLVER=\\\"$$JDLLVER\\\"

macx:CONFIG += c++11

win32-msvc*:TARGET = ../bin/jqt
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp jepath.cpp
win32:SOURCES += dllsrc/jdllcomx.cpp
win32:HEADERS += dllsrc/jexe.h dllsrc/jdllcom.h dllsrc/jdlltype.h
CONFIG+= release

# macOS-specific sources (Objective-C).
macx:OBJECTIVE_SOURCES += disableWindowTabbing.mm
macx:LIBS += -framework AppKit

win32:LIBS += -lole32 -loleaut32 -luuid -ladvapi32
win32-msvc*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32:!win32-msvc*:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_CXXFLAGS += -WX
win32-msvc*:QMAKE_LFLAGS += /STACK:10000000
macx:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
macx:QMAKE_RPATHDIR +=@executable_path/../Qt/Frameworks
win32:RC_FILE = jqt.rc
