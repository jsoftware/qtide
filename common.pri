# included by main.pro and lib.pro

VERSION = 1.8.6

JQTBUILD = "full"
JQTFAT = $$(JQTFAT)
JQTSLIM = $$(JQTSLIM)
!isEmpty(JQTFAT) JQTBUILD = "fat"
!isEmpty(JQTSLIM) JQTBUILD = "slim"

DEFINES += JQTVERSION=\\\"jqtversion:$$JQTBUILD/$$VERSION/$$[QT_VERSION]:\\\"

# for osx - match qmake with xcode. Get version from:
# ls /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/
#QMAKE_MAC_SDK = MacOSX10.13
EXPORT_QMAKE_MAC_SDK = MacOSX10.13
