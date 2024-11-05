
# version info
include(../common.pri)
message(QMAKESPEC $$QMAKESPEC)

!lessThan(QT_MAJOR_VERSION, 5) {
 DEFINES += QT50
 !lessThan(QT_MINOR_VERSION,7): DEFINES += QT57
 !lessThan(QT_MINOR_VERSION,12): DEFINES += QT512
 !lessThan(QT_MINOR_VERSION,15): DEFINES += QT515
}

!lessThan(QT_MAJOR_VERSION, 6) {
 DEFINES += QT60
 !lessThan(QT_MINOR_VERSION,2): DEFINES += QT62
 !lessThan(QT_MINOR_VERSION,8): DEFINES += QT68
 DEFINES += QT57
 DEFINES += QT512
 DEFINES += QT515
}


android {
  CONFIG += mobility
  MOBILITY +=
  TEMPLATE = lib
  TARGET = qtide
  !contains(DEFINES,QT62): error(requires Qt6.2)
} else {
  TEMPLATE = app
  TARGET = jqt
}
# export JQTRPATH=JQTRPATH to enable runpath on linux
JQTRPATH = $$(JQTRPATH)

TEMPLATE = app
TARGET = jqt

message(original arch $$QMAKE_HOST.arch)
QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32: QMAKE_TARGET.arch = x86
linux-g++-64: QMAKE_TARGET.arch = x86_64
linux-cross: QMAKE_TARGET.arch = x86
win32-clang*: QMAKE_TARGET.arch = x86_64
win32-arm64*: QMAKE_TARGET.arch = arm64
# android: QMAKE_TARGET.arch = arm64
linux-raspi: QMAKE_TARGET.arch = armv6l
linux-arm: !linux-arm64: QMAKE_TARGET.arch = armv6l
linux-arm64: QMAKE_TARGET.arch = aarch64
linux-aarch64: QMAKE_TARGET.arch = aarch64
macx-ios: QT -= printsupport
wasm: QT -= printsupport

equals(QMAKE_TARGET.arch , i686): QMAKE_TARGET.arch = x86
equals(QMAKE_TARGET.arch , amd64): QMAKE_TARGET.arch = x86_64
equals(QMAKE_TARGET.arch , arm64): QMAKE_TARGET.arch = aarch64
message(adjusted arch $$QMAKE_TARGET.arch)

ABI=$$(ABI)
android {
!isEmpty(ABI): QMAKE_TARGET.arch = $$ABI
}

equals(QMAKE_TARGET.arch , armv6l): {
  message(building raspberry pi jqt)
  DEFINES += RASPI
  QMAKE_CXXFLAGS += -marm -march=armv6 -mfloat-abi=hard -mfpu=vfp
}

equals(QMAKE_TARGET.arch , aarch64):!macx*:!wasm*:!openbsd:!freebsd:!android:!win32 {
  message(building raspberry pi-3 jqt)
  DEFINES += RASPI
  QMAKE_CXXFLAGS += -march=armv8-a+crc
}

win32: arch = win-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch
macx: arch = mac-$$QMAKE_TARGET.arch
unix:!macx:!andrid: arch = linux-$$QMAKE_TARGET.arch
freebsd: arch = freebsd-$$QMAKE_TARGET.arch
openbsd: arch = openbsd-$$QMAKE_TARGET.arch
macx-ios: arch = ios-$$QMAKE_TARGET.arch
wasm: arch = wasm-$$QMAKE_TARGET.arch

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

macx:CONFIG += c++11
win32:TARGET = ../bin/jqt
win32-msvc*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-arm64*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-clang-msvc:DEFINES += _CRT_SECURE_NO_WARNINGS
DEPENDPATH += .
INCLUDEPATH += .

DEFINES += JDLLVER=\\\"$$JDLLVER\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += BUILD_VERSION=\\\"$$BUILDVERSION\\\"

# Input
SOURCES += main.cpp jepath.cpp
win32:SOURCES += dllsrc/jdllcomx.cpp
win32:HEADERS += dllsrc/jexe.h dllsrc/jdllcom.h dllsrc/jdlltype.h

# macOS-specific sources (Objective-C).
macx:OBJECTIVE_SOURCES += disableWindowTabbing.mm
macx:LIBS += -framework AppKit

win32:LIBS += -lole32 -loleaut32 -luuid -ladvapi32
win32-msvc*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-arm64*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-clang-msvc:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-g++:QMAKE_LFLAGS += -static-libgcc
win32-clang-g++:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_CXXFLAGS += -WX
win32-arm64*:QMAKE_CXXFLAGS += -WX
win32-clang-msvc:QMAKE_CXXFLAGS += -WX
win32-msvc*:QMAKE_LFLAGS += /STACK:0xc00000
win32-mrm64*:QMAKE_LFLAGS += /STACK:0xc00000
win32-clang-msvc:QMAKE_LFLAGS += /STACK:0xc00000
macx:QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
macx:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
macx:QMAKE_RPATHDIR +=@executable_path/../Qt/Frameworks
linux:!isEmpty(JQTRPATH) {
QMAKE_RPATHDIR += $ORIGIN/../Qt/lib
}
!isEmpty(QMAKE_RPATHDIR): message(RPATHDIR = $$QMAKE_RPATHDIR)

win32:RC_FILE = jqt.rc
