
# version info
include(../common.pri)
message(QMAKESPEC $$QMAKESPEC)

# DEFINES += TABCOMPLETION # uncomment this line for tab completion

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
 QT += core5compat
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
TEMPLATE = lib
TARGET = jqt

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
  QT -=  quick qml quickwidgets webengine webenginewidgets webkit webkitwidgets
}


message(original arch $$QMAKE_HOST.arch)
QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32: QMAKE_TARGET.arch = x86
linux-g++-64: QMAKE_TARGET.arch = x86_64
win32-clang*: QMAKE_TARGET.arch = x86_64
win32-arm64*: QMAKE_TARGET.arch = arm64
linux-raspi: QMAKE_TARGET.arch = armv6l
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

win32: arch = win-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch
macx: arch = mac-$$QMAKE_TARGET.arch
linux: arch = linux-$$QMAKE_TARGET.arch
freebsd: arch = freebsd-$$QMAKE_TARGET.arch
openbsd: arch = openbsd-$$QMAKE_TARGET.arch
ios: arch = ios-$$QMAKE_TARGET.arch
android: arch = android-$$QMAKE_TARGET.arch
wasm: arch = wasm-$$QMAKE_TARGET.arch

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
win32:CONFIG += dll console
win32-msvc*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-arm64*:DEFINES += _CRT_SECURE_NO_WARNINGS
win32-clang-msvc:DEFINES += _CRT_SECURE_NO_WARNINGS
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

contains(DEFINES,QT54) {
  android: DEFINES += QT_OPENGL_ES_2
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
  android: DEFINES += QT_OPENGL_ES_2
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

# Input
HEADERS += \
 base/base.h base/bedit.h base/comp.h base/dialog.h base/dirm.h base/dlog.h \
 base/fif.h base/fiw.h base/jsvr.h base/menu.h \
 base/nedit.h base/nmain.h base/note.h base/nside.h base/ntabs.h \
 base/plaintextedit.h base/pcombobox.h \
 base/pnew.h base/proj.h base/psel.h base/qmlje.h base/recent.h base/rsel.h \
 base/snap.h base/spic.h base/state.h base/style.h base/svr.h \
 base/tedit.h base/term.h base/textedit.h base/util.h base/utils.h base/qtstate.h \
 base/eview.h base/widget.h high/high.h high/highj.h \
 grid/qgrid.h grid/qutil.h grid/wgrid.h \
 wd/bitmap.h wd/browser.h wd/button.h wd/child.h wd/clipboard.h wd/cmd.h \
 wd/checkbox.h wd/combobox.h wd/dateedit.h wd/dial.h wd/dspinbox.h wd/dummy.h \
 wd/edit.h wd/editm.h wd/edith.h wd/font.h wd/form.h \
 wd/gl2.h wd/glz.h wd/prtobj.h wd/image.h \
 wd/isidraw.h wd/isigraph.h wd/isigraph2.h wd/isigrid.h \
 wd/layout.h wd/lineedit.h wd/listbox.h \
 wd/menus.h wd/pane.h wd/progressbar.h wd/radiobutton.h \
 wd/slider.h wd/spinbox.h wd/static.h wd/statusbar.h wd/table.h \
 wd/tabs.h wd/tabwidget.h \
 wd/timeedit.h wd/toolbar.h wd/treeview.h wd/wd.h \
 wd/ogl2.h wd/opengl.h wd/opengl2.h \
 wd/webengineview.h wd/webview.h wd/quickview1.h wd/quickview2.h wd/quickwidget.h \
 wd/qwidget.h wd/scrollarea.h wd/scrollbar.h wd/gl2class.h wd/drawobj.h \
 wd/multimedia.h wd/svgview.h wd/svgview2.h

android: HEADERS += base/androidextras.h

contains(DEFINES,QT_NO_OPENGL): HEADERS -= wd/ogl2.h wd/opengl.h wd/opengl2.h
contains(DEFINES,QT_NO_WEBKIT): HEADERS -= wd/webview.h
contains(DEFINES,QT_NO_WEBENGINE): HEADERS -= wd/webengine.h wd/webengineview.h
contains(DEFINES,QT50) {
  contains(DEFINES,QT_NO_QUICKVIEW2): HEADERS -= wd/quickview2.h
  contains(DEFINES,QT_NO_QUICKVIEW1): HEADERS -= wd/quickview1.h
  contains(DEFINES,QT_NO_QUICKVIEW2): contains(DEFINES,QT_NO_QUICKVIEW1): HEADERS -= base/qmlje.h
} else {
  contains(DEFINES,QT_NO_QUICKVIEW1): HEADERS -= wd/quickview1.h wd/quickview2.h base/qmlje.h
  HEADERS -= wd/quickview2.h
}
contains(DEFINES,QT_NO_QUICKWIDGET): HEADERS -= wd/quickwidget.h
contains(DEFINES,QT_NO_MULTIMEDIA): HEADERS -= wd/multimedia.h
contains(DEFINES,QT_NO_PRINTER): HEADERS -= wd/glz.h wd/prtobj.h
contains(DEFINES,QTWEBSOCKET): !contains(QT,websockets): HEADERS += QtWebsocket/compat.h QtWebsocket/QWsServer.h QtWebsocket/QWsSocket.h QtWebsocket/QWsHandshake.h QtWebsocket/QWsFrame.h QtWebsocket/QTlsServer.h QtWebsocket/functions.h QtWebsocket/WsEnums.h
contains(DEFINES,QTWEBSOCKET): HEADERS += base/wssvr.h base/wscln.h
android:HEADERS += base/androidextras.h base/qtjni.h
contains(DEFINES,QT_NO_SVGVIEW): HEADERS -= wd/svgview.h wd/svgview2.h

SOURCES += \
 base/comp.cpp base/bedit.cpp base/dialog.cpp \
 base/dirm.cpp base/dirmx.cpp base/dlog.cpp \
 base/fif.cpp base/fifx.cpp base/fiw.cpp base/jsvr.cpp \
 base/menu.cpp base/menuhelp.cpp \
 base/nedit.cpp base/nmain.cpp base/note.cpp base/nside.cpp base/ntabs.cpp \
 base/plaintextedit.cpp base/pcombobox.cpp \
 base/pnew.cpp base/proj.cpp base/psel.cpp base/qmlje.cpp \
 base/recent.cpp base/rsel.cpp base/run.cpp \
 base/snap.cpp base/spic.cpp base/state.cpp base/statex.cpp \
 base/style.cpp base/svr.cpp base/tedit.cpp base/term.cpp base/textedit.cpp \
 base/userkeys.cpp base/util.cpp base/utils.cpp base/qtstate.cpp base/eview.cpp base/widget.cpp \
 grid/cell.cpp grid/cubedata.cpp grid/cubedraw.cpp grid/cubewidget.cpp \
 grid/defs.cpp grid/draw.cpp grid/header.cpp grid/hierdraw.cpp \
 grid/hierwidget.cpp grid/label.cpp grid/qgrid.cpp grid/qutil.cpp \
 grid/sizes.cpp grid/top.cpp grid/wgrid.cpp high/highj.cpp \
 wd/bitmap.cpp wd/browser.cpp wd/button.cpp wd/child.cpp wd/clipboard.cpp wd/cmd.cpp \
 wd/checkbox.cpp wd/combobox.cpp wd/dateedit.cpp wd/dial.cpp wd/dspinbox.cpp wd/dummy.cpp \
 wd/edit.cpp wd/editm.cpp wd/edith.cpp wd/font.cpp \
 wd/form.cpp wd/gl2.cpp wd/glz.cpp wd/prtobj.cpp wd/image.cpp \
 wd/isidraw.cpp wd/isigraph.cpp wd/isigraph2.cpp wd/isigrid.cpp \
 wd/layout.cpp wd/lineedit.cpp wd/listbox.cpp wd/mb.cpp \
 wd/menus.cpp wd/pane.cpp wd/progressbar.cpp wd/radiobutton.cpp \
 wd/slider.cpp wd/sm.cpp wd/spinbox.cpp wd/static.cpp wd/statusbar.cpp \
 wd/table.cpp wd/tabs.cpp wd/tabwidget.cpp \
 wd/timeedit.cpp wd/toolbar.cpp wd/treeview.cpp wd/wd.cpp \
 wd/ogl2.cpp wd/opengl.cpp wd/opengl2.cpp \
 wd/webengineview.cpp wd/webview.cpp wd/quickview1.cpp wd/quickview2.cpp wd/quickwidget.cpp \
 wd/qwidget.cpp wd/scrollarea.cpp wd/scrollbar.cpp wd/drawobj.cpp \
 wd/multimedia.cpp wd/svgview.cpp wd/svgview2.cpp

android: SOURCES += base/androidextras.cpp

contains(DEFINES,QT_NO_OPENGL): SOURCES -= wd/ogl2.cpp wd/opengl.cpp wd/opengl2.cpp
contains(DEFINES,QT_NO_WEBKIT): SOURCES -= wd/webview.cpp
contains(DEFINES,QT_NO_WEBENGINE): SOURCES -= wd/webengineview.cpp
contains(DEFINES,QT50) {
  contains(DEFINES,QT_NO_QUICKVIEW2): SOURCES -= wd/quickview2.cpp
  contains(DEFINES,QT_NO_QUICKVIEW1): SOURCES -= wd/quickview1.cpp
  contains(DEFINES,QT_NO_QUICKVIEW2): contains(DEFINES,QT_NO_QUICKVIEW1): SOURCES -= base/qmlje.cpp
} else {
  contains(DEFINES,QT_NO_QUICKVIEW1): SOURCES -= wd/quickview1.cpp wd/quickview2.cpp base/qmlje.cpp
  SOURCES -= wd/quickview2.cpp
}
contains(DEFINES,QT_NO_QUICKWIDGET): SOURCES -= wd/quickwidget.cpp
contains(DEFINES,QT_NO_MULTIMEDIA): SOURCES -= wd/multimedia.cpp
contains(DEFINES,QT_NO_PRINTER): SOURCES -= wd/glz.cpp wd/prtobj.cpp
contains(DEFINES,QTWEBSOCKET): !contains(QT,websockets): SOURCES += QtWebsocket/QWsServer.cpp QtWebsocket/QWsSocket.cpp QtWebsocket/QWsHandshake.cpp QtWebsocket/QWsFrame.cpp QtWebsocket/QTlsServer.cpp QtWebsocket/functions.cpp
contains(DEFINES,QTWEBSOCKET): SOURCES += base/wssvr.cpp base/wscln.cpp wd/ws.cpp
android:SOURCES += base/androidextras.cpp base/qtjni.cpp ../main/main.cpp ../main/jepath.cpp
contains(DEFINES,QT_NO_SVGVIEW): SOURCES -= wd/svgview.cpp wd/svgview2.cpp

RESOURCES += lib.qrc
RESOURCES += styles/qdarkstyle/darkstyle.qrc

win32:VERSION =
unix:!openbsd:LIBS += -ldl
android:LIBS += -ldl

win32-g++:QMAKE_LFLAGS += -static-libgcc
win32-clang-g++:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_CXXFLAGS += -WX
win32-arm64*:QMAKE_CXXFLAGS += -WX
win32-clang-msvc:QMAKE_CXXFLAGS += -WX
macx:QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
macx:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
macx:QMAKE_RPATHDIR += @executable_path/../Qt/Frameworks
linux:!isEmpty(JQTRPATH) {
QMAKE_RPATHDIR += $ORIGIN/../Qt/lib
}
!isEmpty(QMAKE_RPATHDIR): message(RPATHDIR = $$QMAKE_RPATHDIR)
# unix:QMAKE_CXXFLAGS += -fno-sized-deallocation
# linux:QMAKE_CXXFLAGS += -fno-sized-deallocation

# shared library
win32-g++:LIBS += -shared
win32-clang-g++:LIBS += -shared
