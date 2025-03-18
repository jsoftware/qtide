
# version info
include(../common.pri)
message(QMAKESPEC $$QMAKESPEC)

# DEFINES += TABCOMPLETION # uncomment this line for tab completion

DEFINES += JQTAMALGAM

DEFINES += QTWEBSOCKET  # comment this line if QtWebsocket is unwanted
# DEFINES += WDCB             # uncomment this line for 11!:x wd interface
# DEFINES += ONEEVENTLOOP     # uncomment this line for single event loop
# DEFINES += NMDIALOG         # uncomment this line for nonmodal dialogbox


!lessThan(QT_MAJOR_VERSION, 5) {
 DEFINES += QT50
 !lessThan(QT_MINOR_VERSION,7): DEFINES += QT57
 !lessThan(QT_MINOR_VERSION,12): DEFINES += QT512
 !lessThan(QT_MINOR_VERSION,15): DEFINES += QT515
}

!lessThan(QT_MAJOR_VERSION, 6) {
 DEFINES += QT60
 !lessThan(QT_MINOR_VERSION,2): DEFINES += QT62
 !lessThan(QT_MINOR_VERSION,5): DEFINES += QT65
 !lessThan(QT_MINOR_VERSION,8): DEFINES += QT68
 DEFINES += QT57
 DEFINES += QT512
 DEFINES += QT515
}

# export JQTWEBKIT before qmake
# use webkit instead of webengine
JQTWEBKIT = $$(JQTWEBKIT)

# export JQTRPATH=JQTRPATH to enable runpath on linux
JQTRPATH = $$(JQTRPATH)

contains(DEFINES,QTWEBSOCKET): contains(DEFINES,QT57) QT += websockets
contains(DEFINES,QTWEBSOCKET): !contains(DEFINES,QT57) QT += network
!lessThan(QT_MAJOR_VERSION, 5): QT += widgets
!lessThan(QT_MAJOR_VERSION, 5): QT += printsupport
!lessThan(QT_MAJOR_VERSION, 4): QT += opengl
!lessThan(QT_MAJOR_VERSION, 5): QT += multimediawidgets
!lessThan(QT_MAJOR_VERSION, 5): QT += svg
!lessThan(QT_MAJOR_VERSION, 5): contains(DEFINES,QT57) QT += webenginewidgets
!lessThan(QT_MAJOR_VERSION, 5): !contains(DEFINES,QT57): QT += webkit webkitwidgets
!isEmpty(JQTWEBKIT) {
message(use webkit instead of webengine)
QT -= webenginewidgets
QT += webkit webkitwidgets
}
lessThan(QT_MAJOR_VERSION, 5): QT += webkit
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

!lessThan(QT_MAJOR_VERSION, 6): QT += openglwidgets

android  {
  !contains(DEFINES,QT62): error(requires Qt6.2)
  CONFIG += mobility
  MOBILITY +=
  QT += opengl
  DEFINES += QT_NO_PRINTER
  DEFINES += SMALL_SCREEN
  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../android
}
contains(DEFINES,QT56) {
  QT += webengine
  QT -= webkit
}
TEMPLATE = app
TARGET = jqta

# to exclude webkit, uncomment the following line
# QT -= webkit webkitwidgets

# to exclude QtWebEngine, uncomment the following line
# QT -= webenginewidgets

# to exclude svgview, uncomment the following line
# QT -= svg

# to print debug messages for calling JDo,  uncomment the following line
# DEFINES += DEBUG_JDO

# export NO_WEBENGINE before qmake
NO_WEBENGINE = $$(NO_WEBENGINE)
!isEmpty(NO_WEBENGINE) {
  message(disable webengine feature)
  DEFINES += QT_NO_WEBENGINE
  DEFINES -= QT_WEBENGINE
  QT -= webenginewidgets webengine
}

# export JQTFAT before qmake
JQTFAT = $$(JQTFAT)
!isEmpty(JQTFAT) {
  message(building fat jqt)
  QT += qml quick quickwidgets
}

# export JQTSLIM before qmake
JQTSLIM = $$(JQTSLIM)
!isEmpty(JQTSLIM) {
  !isEmpty(JQTFAT): error(both FAT and SLIM defined)
  message(building slim jqt)
  QT -=  multimedia multimediawidgets quick qml quickwidgets webengine webenginewidgets webkit webkitwidgets
}


message(original arch $$QMAKE_HOST.arch)
QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32: QMAKE_TARGET.arch = x86
linux-g++-64: QMAKE_TARGET.arch = x86_64
linux-raspi: QMAKE_TARGET.arch = armv6l
win32-clang*: QMAKE_TARGET.arch = x86_64
win32-arm64*: QMAKE_TARGET.arch = arm64
linux-arm: !linux-arm64: QMAKE_TARGET.arch = armv6l
linux-arm64: QMAKE_TARGET.arch = aarch64
linux-aarch64: QMAKE_TARGET.arch = aarch64
wasm: QMAKE_TARGET.arch = wasm32
android: QMAKE_TARGET.arch = $$(ABI)
android: QT -= printsupport
ios: QT -= printsupport
wasm: QT -= printsupport

equals(QMAKE_TARGET.arch , i686): QMAKE_TARGET.arch = x86
equals(QMAKE_TARGET.arch , amd64): QMAKE_TARGET.arch = x86_64
equals(QMAKE_TARGET.arch , arm64): QMAKE_TARGET.arch = aarch64
message(adjusted arch $$QMAKE_TARGET.arch)

ABI=$$(ABI)
android {
  !isEmpty(ABI): QMAKE_TARGET.arch = $$ABI
}

equals(QMAKE_TARGET.arch , armv6l):linux: {
  message(building raspberry pi jqt)
  DEFINES += RASPI
  QMAKE_CXXFLAGS += -marm -march=armv6 -mfloat-abi=hard -mfpu=vfp
}

equals(QMAKE_TARGET.arch , aarch64):linux: {
  message(building raspberry pi-3 jqt)
  DEFINES += RASPI
  QMAKE_CXXFLAGS += -march=armv8-a+crc
}

isEmpty(QMAKE_TARGET.arch): error(empty target arch)

win32: arch = win-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch
macx: arch = mac-$$QMAKE_TARGET.arch
linux: arch = linux-$$QMAKE_TARGET.arch
freebsd: arch = freebsd-$$QMAKE_TARGET.arch
openbsd: arch = openbsd-$$QMAKE_TARGET.arch
ios: arch = ios-$$QMAKE_TARGET.arch
wasm: arch = wasm-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch
message(arch $$arch)

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
# win32:CONFIG += dll console
win32:TARGET = ../bin/jqtamalgam
DEPENDPATH += .
INCLUDEPATH += .

DEFINES += JDLLVER=\\\"$$JDLLVER\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += BUILD_VERSION=\\\"$$BUILDVERSION\\\"

!contains(QT,webenginewidgets) {
  DEFINES += QT_NO_WEBENGINE
  DEFINES -= QT_WEBENGINE
  QT -= webengine
} else {
  DEFINES -= QT_NO_WEBENGINE
  DEFINES += QT_WEBENGINE
}

!contains(QT,webkit) {
  DEFINES += QT_NO_WEBKIT
  DEFINES -= QT_WEBKIT
  QT -= webkitwidgets
} else {
  DEFINES -= QT_NO_WEBKIT
  DEFINES += QT_WEBKIT
}

isEmpty(JQTWEBKIT) {
contains(DEFINES,QT57) {
  DEFINES += QT_NO_WEBKIT
  DEFINES -= QT_WEBKIT
}
}
!contains(QT,opengl) {
  DEFINES += QT_NO_OPENGL
  DEFINES -= QT_OPENGL
  QT -= openglwidgets
} else {
  DEFINES -= QT_NO_OPENGL
  DEFINES += QT_OPENGL
}

# export NO_OPENGL before qmake
NO_OPENGL = $$(NO_OPENGL)
!isEmpty(NO_OPENGL) {
  message(disable opengl feature)
  DEFINES += QT_NO_OPENGL
  DEFINES -= QT_OPENGL QT_OPENGL_ES_2
  QT -= openglwidgets opengl
}

# QT50 or later
  !contains(QT,quick) {
    DEFINES += QT_NO_QUICKVIEW2
    DEFINES -= QT_QUICKVIEW2
    QT -= quickwidgets
  } else {
    DEFINES -= QT_NO_QUICKVIEW2
    DEFINES += QT_QUICKVIEW2
  }

  !contains(QT,declarative) {
    DEFINES += QT_NO_QUICKVIEW1
    DEFINES -= QT_QUICKVIEW1
  } else {
    DEFINES -= QT_NO_QUICKVIEW1
    DEFINES += QT_QUICKVIEW1
  }


!contains(QT,quickwidgets) {
  DEFINES += QT_NO_QUICKWIDGET
  DEFINES -= QT_QUICKWIDGET
} else {
  DEFINES -= QT_NO_QUICKWIDGET
  DEFINES += QT_QUICKWIDGET
}

!contains(QT,multimediawidgets) {
  DEFINES += QT_NO_MULTIMEDIA
  DEFINES -= QT_MULTIMEDIA
  QT -=  multimedia
} else {
  DEFINES -= QT_NO_MULTIMEDIA
  DEFINES += QT_MULTIMEDIA
}

!contains(QT,svg) {
  DEFINES += QT_NO_SVGVIEW
  DEFINES -= QT_SVGVIEW
} else {
  DEFINES -= QT_NO_SVGVIEW
  DEFINES += QT_SVGVIEW
}

!contains(QT,printsupport) {
  DEFINES += QT_NO_PRINTER
  DEFINES -= QT_PRINTER
  DEFINES += QT_NO_PRINTDIALOG
  DEFINES -= QT_PRINTDIALOG
} else {
  DEFINES -= QT_NO_PRINTER
  DEFINES += QT_PRINTER
  DEFINES -= QT_NO_PRINTDIALOG
  DEFINES += QT_PRINTDIALOG
}

# main
# Input
SOURCES += ../main/main.cpp ../main/jepath.cpp
win32:SOURCES += ../main/dllsrc/jdllcomx.cpp
win32:HEADERS += ../main/dllsrc/jexe.h ../main/dllsrc/jdllcom.h ../main/dllsrc/jdlltype.h

# macOS-specific sources (Objective-C).
macx:OBJECTIVE_SOURCES += ../main/disableWindowTabbing.mm
macx:LIBS += -framework AppKit


# lib
# Input
HEADERS += \
 ../lib/base/base.h ../lib/base/bedit.h ../lib/base/comp.h ../lib/base/dialog.h ../lib/base/dirm.h ../lib/base/dlog.h \
 ../lib/base/fif.h ../lib/base/fiw.h ../lib/base/jsvr.h ../lib/base/menu.h \
 ../lib/base/nedit.h ../lib/base/nmain.h ../lib/base/note.h ../lib/base/nside.h ../lib/base/ntabs.h \
 ../lib/base/plaintextedit.h ../lib/base/pcombobox.h \
 ../lib/base/pnew.h ../lib/base/proj.h ../lib/base/psel.h ../lib/base/qmlje.h ../lib/base/recent.h ../lib/base/rsel.h \
 ../lib/base/snap.h ../lib/base/spic.h ../lib/base/state.h ../lib/base/style.h ../lib/base/svr.h \
 ../lib/base/tedit.h ../lib/base/term.h ../lib/base/textedit.h ../lib/base/util.h ../lib/base/utils.h ../lib/base/qtstate.h \
 ../lib/base/eview.h ../lib/base/widget.h ../lib/high/high.h ../lib/high/highj.h \
 ../lib/grid/qgrid.h ../lib/grid/qutil.h ../lib/grid/wgrid.h \
 ../lib/wd/bitmap.h ../lib/wd/browser.h ../lib/wd/button.h ../lib/wd/child.h ../lib/wd/clipboard.h ../lib/wd/cmd.h \
 ../lib/wd/checkbox.h ../lib/wd/combobox.h ../lib/wd/dateedit.h ../lib/wd/dial.h ../lib/wd/dspinbox.h ../lib/wd/dummy.h \
 ../lib/wd/edit.h ../lib/wd/editm.h ../lib/wd/edith.h ../lib/wd/font.h ../lib/wd/form.h \
 ../lib/wd/gl2.h ../lib/wd/glz.h ../lib/wd/prtobj.h ../lib/wd/image.h \
 ../lib/wd/isidraw.h ../lib/wd/isigraph.h ../lib/wd/isigraph2.h ../lib/wd/isigrid.h \
 ../lib/wd/layout.h ../lib/wd/lineedit.h ../lib/wd/listbox.h \
 ../lib/wd/menus.h ../lib/wd/pane.h ../lib/wd/progressbar.h ../lib/wd/radiobutton.h \
 ../lib/wd/slider.h ../lib/wd/spinbox.h ../lib/wd/static.h ../lib/wd/statusbar.h ../lib/wd/table.h \
 ../lib/wd/tabs.h ../lib/wd/tabwidget.h \
 ../lib/wd/timeedit.h ../lib/wd/treeview.h ../lib/wd/toolbar.h ../lib/wd/wd.h \
 ../lib/wd/ogl2.h ../lib/wd/opengl.h ../lib/wd/opengl2.h \
 ../lib/wd/webengineview.h ../lib/wd/webview.h ../lib/wd/quickview1.h ../lib/wd/quickview2.h ../lib/wd/quickwidget.h \
 ../lib/wd/qwidget.h ../lib/wd/scrollarea.h ../lib/wd/scrollbar.h ../lib/wd/gl2class.h ../lib/wd/drawobj.h \
 ../lib/wd/multimedia.h ../lib/wd/svgview.h ../lib/wd/svgview2.h
android:HEADERS += ../lib/base/androidextras.h ../lib/base/qtjni.h

contains(DEFINES,QT_NO_OPENGL): HEADERS -= ../lib/wd/ogl2.h ../lib/wd/opengl.h ../lib/wd/opengl2.h
contains(DEFINES,QT_NO_WEBKIT): HEADERS -= ../lib/wd/webview.h
contains(DEFINES,QT_NO_WEBENGINE): HEADERS -= ../lib/wd/webengine.h ../lib/wd/webengineview.h
contains(DEFINES,QT50) {
  contains(DEFINES,QT_NO_QUICKVIEW2): HEADERS -= ../lib/wd/quickview2.h
  contains(DEFINES,QT_NO_QUICKVIEW1): HEADERS -= ../lib/wd/quickview1.h
  contains(DEFINES,QT_NO_QUICKVIEW2): contains(DEFINES,QT_NO_QUICKVIEW1): HEADERS -= ../lib/base/qmlje.h
} else {
  contains(DEFINES,QT_NO_QUICKVIEW1): HEADERS -= ../lib/wd/quickview1.h ../lib/wd/quickview2.h ../lib/base/qmlje.h
  HEADERS -= ../lib/wd/quickview2.h
}
contains(DEFINES,QT_NO_QUICKWIDGET): HEADERS -= ../lib/wd/quickwidget.h
contains(DEFINES,QT_NO_MULTIMEDIA): HEADERS -= ../lib/wd/multimedia.h
contains(DEFINES,QT_NO_PRINTER): HEADERS -= ../lib/wd/glz.h ../lib/wd/prtobj.h
contains(DEFINES,QTWEBSOCKET): !contains(QT,websockets): HEADERS += ../lib/QtWebsocket/compat.h ../lib/QtWebsocket/QWsServer.h ../lib/QtWebsocket/QWsSocket.h ../lib/QtWebsocket/QWsHandshake.h ../lib/QtWebsocket/QWsFrame.h ../lib/QtWebsocket/QTlsServer.h ../lib/QtWebsocket/functions.h ../lib/QtWebsocket/WsEnums.h
contains(DEFINES,QTWEBSOCKET): HEADERS += ../lib/base/wssvr.h ../lib/base/wscln.h
contains(DEFINES,QT_NO_SVGVIEW): HEADERS -= ../lib/wd/svgview.h ../lib/wd/svgview2.h

SOURCES += \
 ../lib/base/comp.cpp ../lib/base/bedit.cpp ../lib/base/dialog.cpp \
 ../lib/base/dirm.cpp ../lib/base/dirmx.cpp ../lib/base/dlog.cpp \
 ../lib/base/fif.cpp ../lib/base/fifx.cpp ../lib/base/fiw.cpp ../lib/base/jsvr.cpp \
 ../lib/base/menu.cpp ../lib/base/menuhelp.cpp \
 ../lib/base/nedit.cpp ../lib/base/nmain.cpp ../lib/base/note.cpp ../lib/base/nside.cpp ../lib/base/ntabs.cpp \
 ../lib/base/plaintextedit.cpp ../lib/base/pcombobox.cpp \
 ../lib/base/pnew.cpp ../lib/base/proj.cpp ../lib/base/psel.cpp ../lib/base/qmlje.cpp \
 ../lib/base/recent.cpp ../lib/base/rsel.cpp ../lib/base/run.cpp \
 ../lib/base/snap.cpp ../lib/base/spic.cpp ../lib/base/state.cpp ../lib/base/statex.cpp \
 ../lib/base/style.cpp ../lib/base/svr.cpp ../lib/base/tedit.cpp ../lib/base/term.cpp ../lib/base/textedit.cpp \
 ../lib/base/userkeys.cpp ../lib/base/util.cpp ../lib/base/utils.cpp ../lib/base/qtstate.cpp ../lib/base/eview.cpp ../lib/base/widget.cpp \
 ../lib/grid/cell.cpp ../lib/grid/cubedata.cpp ../lib/grid/cubedraw.cpp ../lib/grid/cubewidget.cpp \
 ../lib/grid/defs.cpp ../lib/grid/draw.cpp ../lib/grid/header.cpp ../lib/grid/hierdraw.cpp \
 ../lib/grid/hierwidget.cpp ../lib/grid/label.cpp ../lib/grid/qgrid.cpp ../lib/grid/qutil.cpp \
 ../lib/grid/sizes.cpp ../lib/grid/top.cpp ../lib/grid/wgrid.cpp ../lib/high/highj.cpp \
 ../lib/wd/bitmap.cpp ../lib/wd/browser.cpp ../lib/wd/button.cpp ../lib/wd/child.cpp ../lib/wd/clipboard.cpp ../lib/wd/cmd.cpp \
 ../lib/wd/checkbox.cpp ../lib/wd/combobox.cpp ../lib/wd/dateedit.cpp ../lib/wd/dial.cpp ../lib/wd/dspinbox.cpp ../lib/wd/dummy.cpp \
 ../lib/wd/edit.cpp ../lib/wd/editm.cpp ../lib/wd/edith.cpp ../lib/wd/font.cpp \
 ../lib/wd/form.cpp ../lib/wd/gl2.cpp ../lib/wd/glz.cpp ../lib/wd/prtobj.cpp ../lib/wd/image.cpp \
 ../lib/wd/isidraw.cpp ../lib/wd/isigraph.cpp ../lib/wd/isigraph2.cpp ../lib/wd/isigrid.cpp \
 ../lib/wd/layout.cpp ../lib/wd/lineedit.cpp ../lib/wd/listbox.cpp ../lib/wd/mb.cpp \
 ../lib/wd/menus.cpp ../lib/wd/pane.cpp ../lib/wd/progressbar.cpp ../lib/wd/radiobutton.cpp \
 ../lib/wd/slider.cpp ../lib/wd/sm.cpp ../lib/wd/spinbox.cpp ../lib/wd/static.cpp ../lib/wd/statusbar.cpp \
 ../lib/wd/table.cpp ../lib/wd/tabs.cpp ../lib/wd/tabwidget.cpp \
 ../lib/wd/timeedit.cpp ../lib/wd/treeview.cpp ../lib/wd/toolbar.cpp ../lib/wd/wd.cpp \
 ../lib/wd/ogl2.cpp ../lib/wd/opengl.cpp ../lib/wd/opengl2.cpp \
 ../lib/wd/webengineview.cpp ../lib/wd/webview.cpp ../lib/wd/quickview1.cpp ../lib/wd/quickview2.cpp ../lib/wd/quickwidget.cpp \
 ../lib/wd/qwidget.cpp ../lib/wd/scrollarea.cpp ../lib/wd/scrollbar.cpp ../lib/wd/drawobj.cpp \
 ../lib/wd/multimedia.cpp ../lib/wd/svgview.cpp ../lib/wd/svgview2.cpp
android:SOURCES += ../lib/base/androidextras.cpp ../lib/base/qtjni.cpp

contains(DEFINES,QT_NO_OPENGL): SOURCES -= ../lib/wd/ogl2.cpp ../lib/wd/opengl.cpp ../lib/wd/opengl2.cpp
contains(DEFINES,QT_NO_WEBKIT): SOURCES -= ../lib/wd/webview.cpp
contains(DEFINES,QT_NO_WEBENGINE): SOURCES -= ../lib/wd/webengineview.cpp
contains(DEFINES,QT50) {
  contains(DEFINES,QT_NO_QUICKVIEW2): SOURCES -= ../lib/wd/quickview2.cpp
  contains(DEFINES,QT_NO_QUICKVIEW1): SOURCES -= ../lib/wd/quickview1.cpp
  contains(DEFINES,QT_NO_QUICKVIEW2): contains(DEFINES,QT_NO_QUICKVIEW1): SOURCES -= ../lib/base/qmlje.cpp
} else {
  contains(DEFINES,QT_NO_QUICKVIEW1): SOURCES -= ../lib/wd/quickview1.cpp ../lib/wd/quickview2.cpp ../lib/base/qmlje.cpp
  SOURCES -= ../lib/wd/quickview2.cpp
}
contains(DEFINES,QT_NO_QUICKWIDGET): SOURCES -= ../lib/wd/quickwidget.cpp
contains(DEFINES,QT_NO_MULTIMEDIA): SOURCES -= ../lib/wd/multimedia.cpp
contains(DEFINES,QT_NO_PRINTER): SOURCES -= ../lib/wd/glz.cpp ../lib/wd/prtobj.cpp
contains(DEFINES,QTWEBSOCKET): !contains(QT,websockets): SOURCES += ../lib/QtWebsocket/QWsServer.cpp ../lib/QtWebsocket/QWsSocket.cpp ../lib/QtWebsocket/QWsHandshake.cpp ../lib/QtWebsocket/QWsFrame.cpp ../lib/QtWebsocket/QTlsServer.cpp ../lib/QtWebsocket/functions.cpp
contains(DEFINES,QTWEBSOCKET): SOURCES += ../lib/base/wssvr.cpp ../lib/base/wscln.cpp ../lib/wd/ws.cpp
contains(DEFINES,QT_NO_SVGVIEW): SOURCES -= ../lib/wd/svgview.cpp ../lib/wd/svgview2.cpp

RESOURCES += ../lib/lib.qrc
RESOURCES += ../lib/styles/qdarkstyle/darkstyle.qrc

win32:VERSION =
unix:!openbsd:LIBS += -ldl
android:LIBS += -ldl

ios{
LIBS -= -ldl
DEFINES += WDCB
RESOURCES += ../jlibrary.qrc
RESOURCES += ../test.qrc
OBJECTIVE_HEADERS += ../main/redminedevicehelper.h
OBJECTIVE_SOURCES += ../main/redminedevicehelper.mm
LIBS += -framework Foundation -framework CoreFoundation -framework UIKit
# macOS-specific sources (Objective-C).
CONFIG(iphoneos,iphoneos|iphonesimulator):message(ios iphoneos)
CONFIG(iphonesimulator,iphoneos|iphonesimulator):message(ios iphonesimulator)
CONFIG(iphoneos,iphoneos|iphonesimulator):QMAKE_LFLAGS += -L../ios/j64iphoneos -lj -L../ios/mpir -lgmp
CONFIG(iphonesimulator,iphoneos|iphonesimulator):QMAKE_LFLAGS += -L../ios/j64iphonesimulator -lj -L../ios/mpir -lgmp
QMAKE_IOS_DEPLOYMENT_TARGET = 13.0
disable_warning.name = GCC_WARN_64_TO_32_BIT_CONVERSION
disable_warning.value = NO
QMAKE_MAC_XCODE_SETTINGS += disable_warning
# ios_signature.pri contains private information including the following
#   QMAKE_XCODE_CODE_SIGN_IDENTITY = "iPhone Developer"
#   development_team.name = DEVELOPMENT_TEAM
#   development_team.value = <developer id>
#   QMAKE_MAC_XCODE_SETTINGS += development_team
include(../ios_signature.pri)
# Note for devices: 1=iPhone, 2=iPad, 1,2=Universal.
QMAKE_APPLE_TARGETED_DEVICE_FAMILY = 1,2
}
wasm {
LIBS -= -ldl
DEFINES += WDCB
DEFINES += ONEEVENTLOOP
DEFINES += NMDIALOG
RESOURCES += ../jlibrary.qrc
RESOURCES += ../test.qrc
QMAKE_LFLAGS += -L../wasm/j32 -lj -L../wasm/mpir -lgmp
QMAKE_LFLAGS += -sERROR_ON_UNDEFINED_SYMBOLS=0
QMAKE_LFLAGS += -Wl,--shared-memory,--no-check-features
# QMAKE_LFLAGS += -s WASM=1 -s ASSERTIONS=1 -s INITIAL_MEMORY=220MB -s TOTAL_MEMORY=600MB -s ALLOW_MEMORY_GROWTH=1 -s STACK_SIZE=984KB
QMAKE_LFLAGS += -s WASM=1 -s ASSERTIONS=1 -s INITIAL_MEMORY=220MB -s TOTAL_MEMORY=600MB -s STACK_SIZE=984KB
QMAKE_LFLAGS += -s BINARYEN_EXTRA_PASSES="--pass-arg=max-func-params@80" -s EMULATE_FUNCTION_POINTER_CASTS=1 -s NO_EXIT_RUNTIME=1
}
android {
!contains(DEFINES,QT62): error(requires Qt6.2)
CONFIG += mobility
ANDROID_TARGET_SDK_VERSION = 23
ANDROID_TARGET_ARCH = $$(ABI)

ANDROID_EXTRA_LIBS += $$PWD/../android/libs/$$ANDROID_TARGET_ARCH/libgmp.so $$PWD/../android/libs/$$ANDROID_TARGET_ARCH/libjpcre2.so $$PWD/../android/libs/$$ANDROID_TARGET_ARCH/libtsdll.so
LIBS += $$PWD/../android/lib/$$ANDROID_TARGET_ARCH/libj.a

message(ANDROID_EXTRA_LIBS  $$ANDROID_EXTRA_LIBS)
message(LIBS  $$LIBS)
DEPLOYMENTFOLDERS += assets 

QT_ANDROID_PACKAGE_SOURCE_DIR = $HOME/Qt/6.6.1/android_$$ANDROID_TARGET_ARCH/src/android
message(QT_ANDROID_PACKAGE_SOURCE_DIR $$QT_ANDROID_PACKAGE_SOURCE_DIR)

equals(ANDROID_TARGET_ARCH  , arm64-v8a ): QMAKE_CXXFLAGS += -march=armv8-a+crc
equals(ANDROID_TARGET_ARCH  , armeabi-v7a ): QMAKE_CXXFLAGS += -march=armv7-a -mfloat-abi=softfp
equals(ANDROID_TARGET_ARCH  , x86_64 ): QMAKE_CXXFLAGS += -march=x86-64 -msse4.2
equals(ANDROID_TARGET_ARCH  , x86 ): QMAKE_CXXFLAGS += -march=i686 -mssse3 -mfpmath=sse
QMAKE_LFLAGS += -fopenmp -static-openmp

QT += opengl
DEFINES += QT_OPENGL_ES_2
DEFINES += WDCB
DEFINES += ONEEVENTLOOP
DEFINES += NMDIALOG
DEFINES += QT_NO_PRINTER
DEFINES += SMALL_SCREEN
RESOURCES += ../jlibrary.qrc
RESOURCES += ../test.qrc
}

win32-msvc*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-arm64*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-clang-msvc:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-g++:QMAKE_LFLAGS += -static-libgcc
win32-clang-g++:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_CXXFLAGS += -WX
win32-arm64*:QMAKE_CXXFLAGS += -WX
win32-clang-msvc:QMAKE_CXXFLAGS += -WX
macx:QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
macx:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
macx:QMAKE_RPATHDIR +=@executable_path/../Qt/Frameworks
linux:!isEmpty(JQTRPATH) {
QMAKE_RPATHDIR += $ORIGIN/../Qt/lib
}
!isEmpty(QMAKE_RPATHDIR): message(RPATHDIR = $$QMAKE_RPATHDIR)
# unix:QMAKE_CXXFLAGS += -fno-sized-deallocation

# executable
win32:LIBS += -lole32 -loleaut32 -luuid -ladvapi32
win32:RC_FILE = ../main/jqt.rc
win32-msvc*:QMAKE_LFLAGS += /STACK:0xc00000
win32-arm64*:QMAKE_LFLAGS += /STACK:0xc00000
win32-clang-msvc:QMAKE_LFLAGS += /STACK:0xc00000
