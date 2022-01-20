
# version info
include(../common.pri)

!lessThan(QT_MAJOR_VERSION, 5) {
 DEFINES += QT50
 !lessThan(QT_MINOR_VERSION,7): DEFINES += QT57
 !lessThan(QT_MINOR_VERSION,12): DEFINES += QT512
 !lessThan(QT_MINOR_VERSION,15): DEFINES += QT515
}

!lessThan(QT_MAJOR_VERSION, 6) {
 DEFINES += QT60
 !lessThan(QT_MINOR_VERSION,2): DEFINES += QT62
 DEFINES += QT57
 DEFINES += QT512
 DEFINES += QT515
}

TEMPLATE = app
TARGET = jqt

# contains(DEFINES,QT50): QT += widgets

QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32: QMAKE_TARGET.arch = x86
linux-g++-64: QMAKE_TARGET.arch = x86_64
linux-cross: QMAKE_TARGET.arch = x86
win32-cross-32: QMAKE_TARGET.arch = x86
win32-cross: QMAKE_TARGET.arch = x86_64
linux-raspi: QMAKE_TARGET.arch = armv6l
linux-arm*: QMAKE_TARGET.arch = armv6l
linux-aarch64*: QMAKE_TARGET.arch = aarch64

equals(QMAKE_TARGET.arch , i686): QMAKE_TARGET.arch = x86
ABI=$$(ABI)

equals(QMAKE_TARGET.arch , armv6l): {
  message(building raspberry pi jqt)
  DEFINES += RASPI
  QMAKE_CXXFLAGS += -marm -march=armv6 -mfloat-abi=hard -mfpu=vfp
}

equals(QMAKE_TARGET.arch , aarch64):!macx: {
  message(building raspberry pi-3 jqt)
  DEFINES += RASPI
  QMAKE_CXXFLAGS += -march=armv8-a+crc
}

win32: arch = win-$$QMAKE_TARGET.arch
macx: arch = mac-$$QMAKE_TARGET.arch
unix:!macx: arch = linux-$$QMAKE_TARGET.arch

# uncomment the next to open windows console to display qDebug() messages
# win32:CONFIG += console

CONFIG+= release
# CONFIG+= debug
CONFIG(debug, debug|release) {
  rel = debug
} else {
  rel = release
}

BUILDROOT = build/$$arch/$$rel
TARGETROOT = ../bin/$$arch/$$rel
DESTDIR = $$TARGETROOT
DLLDESTDIR = $$TARGETROOT

OBJECTS_DIR = $$BUILDROOT/obj
MOC_DIR = $$BUILDROOT/moc
RCC_DIR = $$BUILDROOT/rcc
UI_DIR = $$BUILDROOT/ui

DEFINES += JDLLVER=\\\"$$JDLLVER\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

macx:CONFIG += c++11

win32-msvc*:TARGET = ../bin/jqt
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp jepath.cpp
win32:SOURCES += dllsrc/jdllcomx.cpp
win32:HEADERS += dllsrc/jexe.h dllsrc/jdllcom.h dllsrc/jdlltype.h

# macOS-specific sources (Objective-C).
macx:OBJECTIVE_SOURCES += disableWindowTabbing.mm
macx:LIBS += -framework AppKit

win32:LIBS += -lole32 -loleaut32 -luuid -ladvapi32
win32-msvc*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32:!win32-msvc*:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_CXXFLAGS += -WX
win32-msvc*:QMAKE_LFLAGS += /STACK:12000000
macx:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
macx:QMAKE_RPATHDIR +=@executable_path/../Qt/Frameworks
win32:RC_FILE = jqt.rc
