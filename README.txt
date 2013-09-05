
The jqtide application is an executable, jqt, and a shared object, libjqt.so.

Folders:

bin   - binaries, demo files
config - example configs
lib   - source for libjqt.so (in subdirs base, high and wd)
main  - source for jqt
android - source for android

Build:

Edit makeall.sh in the top level directory to set jqt path, then run.

Copy the binaries and links from bin to the j8/bin directory. 

Edit config files and copy to the J8 config directory

Run:

Example shell script:

#!/bin/bash
J=~/j8
cd $J
bin/jqt "$@"
