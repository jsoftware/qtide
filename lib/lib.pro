
android:{
          CONFIG += mobility
          MOBILITY +=
          TEMPLATE = lib
          TARGET = ../bin/jqt
          DEFINES += "ANDROID" }
else {    TEMPLATE = lib
          TARGET = ../bin/jqt }

OBJECTS_DIR = build
MOC_DIR = build

win32:CONFIG += dll console
QT += webkit
QT += opengl
CONFIG+= release
DEPENDPATH += .
INCLUDEPATH += .

DEFINES += "JQT"
greaterThan(QT_VERSION,4.8.0): DEFINES += QT48

# Input
HEADERS += \
 base/base.h base/bedit.h base/comp.h base/dialog.h base/dirm.h base/dlog.h \
 base/fif.h base/fiw.h base/jsvr.h base/menu.h \
 base/nedit.h base/nmain.h base/note.h base/nside.h base/ntabs.h \
 base/pnew.h base/proj.h base/psel.h base/recent.h base/rsel.h \
 base/snap.h base/spic.h base/state.h base/svr.h \
 base/tedit.h base/term.h base/util.h base/utils.h \
 base/view.h base/widget.h \
 high/high.h high/highj.h \
 wd/bitmap.h wd/button.h wd/child.h wd/clipboard.h wd/cmd.h wd/checkbox.h \
 wd/combobox.h wd/dummy.h wd/edit.h wd/editm.h wd/font.h wd/form.h wd/gl2.h \
 wd/isigraph.h wd/isigraph2.h wd/opengl.h wd/opengl2.h \
 wd/listbox.h wd/radiobutton.h wd/static.h wd/table.h wd/webview.h \
 wd/menus.h wd/wd.h

android:HEADERS += base/qtjni.h

SOURCES += \
 base/comp.cpp base/bedit.cpp base/dialog.cpp \
 base/dirm.cpp base/dirmx.cpp base/dlog.cpp \
 base/fif.cpp base/fifx.cpp base/fiw.cpp base/jsvr.cpp \
 base/menu.cpp base/menuhelp.cpp \
 base/nedit.cpp base/nmain.cpp base/note.cpp base/nside.cpp base/ntabs.cpp \
 base/pnew.cpp base/proj.cpp base/psel.cpp base/recent.cpp base/rsel.cpp \
 base/run.cpp base/snap.cpp base/spic.cpp base/state.cpp base/statex.cpp \
 base/svr.cpp base/tedit.cpp base/term.cpp \
 base/util.cpp base/utils.cpp \
 base/view.cpp base/widget.cpp \
 high/highj.cpp \
 wd/bitmap.cpp wd/button.cpp wd/child.cpp wd/clipboard.cpp wd/cmd.cpp wd/checkbox.cpp \
 wd/combobox.cpp wd/dummy.cpp wd/edit.cpp wd/editm.cpp wd/font.cpp wd/form.cpp wd/gl2.cpp \
 wd/isigraph.cpp wd/isigraph2.cpp wd/opengl.cpp wd/opengl2.cpp \
 wd/listbox.cpp wd/radiobutton.cpp wd/static.cpp wd/table.cpp wd/webview.cpp \
 wd/menus.cpp wd/wd.cpp

android:SOURCES += base/qtjni.cpp ../main/main.cpp

RESOURCES += lib.qrc

win32:LIBS += -shared
unix:LIBS += -ldl
android:LIBS += -ldl
android:LIBS += -lGLESv2

win32:QMAKE_LFLAGS += -static-libgcc

