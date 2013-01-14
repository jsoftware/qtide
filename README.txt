
The jqtide application is an executable, jqt, and a shared object, libjqt.so.

Folders:

bin   - binaries, demo files
config - example configs
lib   - source for libjqt.so (in subdirs base, high and wd)
main  - source for jqt

Build:

Edit makeall.sh in the top level directory to set jqt path, then run.

Copy the binaries and links from bin to the j7/bin directory. 

Edit config files and copy to the J7 config directory

Run:

Example shell script:

#!/bin/bash
J=~/j7
cd $J
bin/jqt "$@"

----------------------------------------

Android specific information:

This jqt Android port is experimental only.

Here it is assumed building for armv5, it will be similar for armv7.

The jqtide application is a shared object, libqtide.so.  There is no jqt executable.

The folder android contains the android java project files.

Download and install the latest sdk from http://necessitas.kde.org

Either copy the folder mkspecs/android-g++ inside necessitas sdk to your system's mkspecs in order to use system's qmake, 

Or edit makeandroid.sh in the top level directory to use qmake from necessitas sdk.

In the android folder, copy the file local.properties_template to local.properties and edit the sdk.dir to point to android sdk.

Copy pre-built libj.so and libjpcre.so to android/libs/armeabi

Make a tarball jqtdata.tgz from all folders under J's ~install folder that should be bundled and copy it to android/assets.

increase the version number in android/assets/assets_version.txt if jqtdata.tgz has been updated.

After running makeandroid.sh, change directory to android, and build apk using the ant command.

$ ant release

this will build the qtide-release-unsigned.apk in the android/bin folder.

Optional: sign the apk with your own secret key.
