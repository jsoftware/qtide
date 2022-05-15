
# version info
include(../common.pri)
message($$QMAKESPEC)

# DEFINES += TABCOMPLETION # uncomment this line for tab completion

DEFINES += QTWEBSOCKET  # comment this line if QtWebsocket is unwanted


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
 DEFINES += QT57
 DEFINES += QT512
 DEFINES += QT515
}

contains(DEFINES,QTWEBSOCKET): contains(DEFINES,QT57) QT += websockets
contains(DEFINES,QTWEBSOCKET): !contains(DEFINES,QT57) QT += network
!lessThan(QT_MAJOR_VERSION, 5): QT += widgets
!lessThan(QT_MAJOR_VERSION, 5): QT += printsupport
!lessThan(QT_MAJOR_VERSION, 4): QT += opengl
!lessThan(QT_MAJOR_VERSION, 5): QT += multimediawidgets
!lessThan(QT_MAJOR_VERSION, 5): contains(DEFINES,QT57) QT += webenginewidgets
!lessThan(QT_MAJOR_VERSION, 5): QT += svg
!lessThan(QT_MAJOR_VERSION, 5): !contains(DEFINES,QT57): QT += webkit webkitwidgets
lessThan(QT_MAJOR_VERSION, 5): QT += webkit

!lessThan(QT_MAJOR_VERSION, 6): QT += openglwidgets

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


QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32: QMAKE_TARGET.arch = x86
linux-g++-64: QMAKE_TARGET.arch = x86_64
linux-cross: QMAKE_TARGET.arch = x86
win32-cross-32: QMAKE_TARGET.arch = x86
win32-cross: QMAKE_TARGET.arch = x86_64
linux-raspi: QMAKE_TARGET.arch = armv6l
linux-arm*: !linux-arm64: QMAKE_TARGET.arch = armv6l
linux-arm64: QMAKE_TARGET.arch = aarch64
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
DEPENDPATH += .
INCLUDEPATH += .

DEFINES += JDLLVER=\\\"$$JDLLVER\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

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

contains(DEFINES,QT57) {
  DEFINES += QT_NO_WEBKIT
  DEFINES -= QT_WEBKIT
}
!contains(QT,opengl) {
  DEFINES += QT_NO_OPENGL
  DEFINES -= QT_OPENGL
  QT -= openglwidgets
} else {
  DEFINES -= QT_NO_OPENGL
  DEFINES += QT_OPENGL
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
 base/plaintextedit.h \
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
 wd/timeedit.h wd/toolbar.h wd/wd.h \
 wd/ogl2.h wd/opengl.h wd/opengl2.h \
 wd/webengineview.h wd/webview.h wd/quickview1.h wd/quickview2.h wd/quickwidget.h \
 wd/qwidget.h wd/scrollarea.h wd/scrollbar.h wd/gl2class.h wd/drawobj.h wd/glc.h \
 wd/multimedia.h wd/svgview.h wd/svgview2.h

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
contains(DEFINES,QT_NO_SVGVIEW): HEADERS -= wd/svgview.h wd/svgview2.h

SOURCES += \
 base/comp.cpp base/bedit.cpp base/dialog.cpp \
 base/dirm.cpp base/dirmx.cpp base/dlog.cpp \
 base/fif.cpp base/fifx.cpp base/fiw.cpp base/jsvr.cpp \
 base/menu.cpp base/menuhelp.cpp \
 base/nedit.cpp base/nmain.cpp base/note.cpp base/nside.cpp base/ntabs.cpp \
 base/plaintextedit.cpp \
 base/pnew.cpp base/proj.cpp base/psel.cpp base/qmlje.cpp \
 base/recent.cpp base/rsel.cpp base/run.cpp \
 base/snap.cpp base/spic.cpp base/state.cpp base/statex.cpp \
 base/style.cpp base/svr.cpp base/tedit.cpp base/term.cpp  base/textedit.cpp \
 base/userkeys.cpp base/util.cpp base/utils.cpp base/qtstate.cpp base/eview.cpp base/widget.cpp \
 grid/cell.cpp grid/cubedata.cpp grid/cubedraw.cpp grid/cubewidget.cpp \
 grid/defs.cpp grid/draw.cpp grid/header.cpp grid/hierdraw.cpp \
 grid/hierwidget.cpp grid/label.cpp grid/qgrid.cpp grid/qutil.cpp \
 grid/sizes.cpp grid/top.cpp grid/wgrid.cpp high/highj.cpp \
 wd/bitmap.cpp wd/browser.cpp wd/button.cpp wd/child.cpp wd/clipboard.cpp wd/cmd.cpp \
 wd/checkbox.cpp wd/combobox.cpp wd/dateedit.cpp wd/dial.cpp wd/dspinbox.cpp wd/dummy.cpp \
 wd/edit.cpp wd/editm.cpp wd/edith.cpp wd/font.cpp \
 wd/form.cpp wd/gl2.cpp wd/glz.cpp wd/prtobj.cpp wd/image.cpp  \
 wd/isidraw.cpp wd/isigraph.cpp wd/isigraph2.cpp wd/isigrid.cpp \
 wd/layout.cpp wd/lineedit.cpp wd/listbox.cpp wd/mb.cpp \
 wd/menus.cpp wd/pane.cpp wd/progressbar.cpp wd/radiobutton.cpp \
 wd/slider.cpp wd/sm.cpp wd/spinbox.cpp wd/static.cpp wd/statusbar.cpp \
 wd/table.cpp wd/tabs.cpp wd/tabwidget.cpp \
 wd/timeedit.cpp wd/toolbar.cpp wd/wd.cpp \
 wd/ogl2.cpp wd/opengl.cpp wd/opengl2.cpp \
 wd/webengineview.cpp wd/webview.cpp wd/quickview1.cpp wd/quickview2.cpp wd/quickwidget.cpp \
 wd/qwidget.cpp wd/scrollarea.cpp wd/scrollbar.cpp wd/drawobj.cpp wd/glc.cpp \
 wd/multimedia.cpp wd/svgview.cpp wd/svgview2.cpp

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
contains(DEFINES,QT_NO_SVGVIEW): SOURCES -= wd/svgview.cpp wd/svgview2.cpp

RESOURCES += lib.qrc
RESOURCES += styles/qdarkstyle/style.qrc

win32:VERSION =
win32:!win32-msvc*:LIBS += -shared
unix:LIBS += -ldl

win32:!win32-msvc*:QMAKE_LFLAGS += -static-libgcc
win32-msvc*:QMAKE_CXXFLAGS += -WX
win32-msvc*:QMAKE_LFLAGS +=
macx:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
macx:QMAKE_RPATHDIR += @executable_path/../Qt/Frameworks
# unix:QMAKE_CXXFLAGS += -fno-sized-deallocation
