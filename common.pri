# included by main.pro and lib.pro

VERSION = 2.0.3
JDLLVER = 9.04    # ignored if not FHS

JQTBUILD = "full"
JQTFAT = $$(JQTFAT)
JQTSLIM = $$(JQTSLIM)
!isEmpty(JQTFAT) JQTBUILD = "fat"
!isEmpty(JQTSLIM) JQTBUILD = "slim"

DEFINES += JQTVERSION=\\\"jqtversion:$$JQTBUILD/$$VERSION/$$[QT_VERSION]:\\\"

# for osx - match qmake with xcode. Get version from:
# ls /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/
#QMAKE_MAC_SDK = MacOSX10.15
EXPORT_QMAKE_MAC_SDK = MacOSX10.15
