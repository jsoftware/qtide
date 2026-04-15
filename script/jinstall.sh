#!/usr/bin/env sh

set -eu

V=j9.7

OS=$(uname -s)
ARCH=$(uname -m)
if [ "$OS" != "Darwin" ] && [ "$OS" != "Linux" ] && [ "$OS" != "FreeBSD" ] && [ "$OS" != "OpenBSD" ]; then
 printf "This script only works for Linux macOS FreeBSD or OpenBSD, not for $OS\n"
 exit 1
fi

# ----------------------------------------------------------------------
# Show usage
usage() {
 cat <<EOF
Usage: $(basename "$0") [OPTIONS]

Options:
  -h --help       Show this help and exit
  -p --path       installation top level directory, default $HOME.
                  The installation is to a $V subdirectory of this.
                  In Linux, a directory of /usr makes a standard system install.
                  Some paths may require root access.
  -q --qt         type of Jqt installed, one of [full]|slim|none
 --no-addons      do not install the full set of addons
 --no-shortcuts   do not create desktop shortcuts
EOF
}

# ----------------------------------------------------------------------
# Parse options
D=$HOME
P=full
A=all
S=true

while [ $# -gt 0 ]; do
 case "$1" in
  -h|--help)      usage;exit 1;;
  -p|--path)      D="$2";shift 2;;
  -q|--qt)        P="$2";shift 2;;
  --no-addons)    A=none;shift;;
  --no-shortcuts) S=false;shift;;
  --) break;;
  *) echo "Unknown option: $1";exit 1;;
 esac
done

#echo "path      = $D"
#echo "addons    = $A"
#echo "jqt       = $P"
#echo "shortcuts = $S"

# check directory
case "$D" in
 none|slim|full)
 printf "The installation directory may not be one of the keywords: none|slim|full\n";
 exit 1 ;;
esac

if [ "$D" = "home" ]; then D="$HOME"; fi

# check Jqt selection
case "$P" in
 none|slim|full) ;;
 *) printf "Invalid Jqt selection: $P\n"; exit 1 ;;
esac

# ----------------------------------------------------------------------
# check user/path/OS combo
UID1=`id -u`
if [ "$D" = "/usr" ]; then
 if [ "$OS" = "Darwin" ]; then
  printf "This script does not support install to /usr in macOS\n"
  exit 1
 fi
 if [ ! $UID1 = 0 ]; then
  printf "Run this script as root to install under /usr\n"
  exit 1
 fi
else
 if [ $UID1 = 0 ]; then
  printf "You are running this script as root. OK to continue? (y/N) "
  read response
  case "${response:-N}" in
   [Yy]*) ;;
   *) exit 1 ;;
  esac
 fi
fi

# ----------------------------------------------------------------------
# resolve directory path
if [ ! "$D" = "/usr" ]; then
 mkdir -p $D
 cd $D
 D=`pwd`
fi

# ----------------------------------------------------------------------
M=$(mktemp -d -t 'jtemp.XXXXXX')
trap 'rm -rf "$M"' EXIT
cd $M

L="https://www.jsoftware.com/download/$V/install"

if [ "$OS" = "Darwin" ]; then
 W=${V}_mac.zip
 curl -OL $L/$W
 unzip $W
else
 if [ "$OS" = "Linux" ]; then
  if [ "$ARCH" = "aarch64" ] || [ "$ARCH" = "arm64" ] ; then
   W=${V}_raspi.tar.gz
  elif [ "$ARCH" = "armv6l" ]; then
   W=${V}_raspi32.tar.gz
  else
   W=${V}_linux.tar.gz
  fi
 elif [ "$OS" = "FreeBSD" ]; then
  if [ "$ARCH" = "aarch64" ] || [ "$ARCH" = "arm64" ] ; then
   printf "This script only works for x86_64\n" ; exit 1
  else
   W=${V}_fbsd.tar.gz
  fi
 elif [ "$OS" = "OpenBSD" ]; then
  if [ "$ARCH" = "aarch64" ] || [ "$ARCH" = "arm64" ] ; then
   printf "This script only works for x86_64\n" ; exit 1
  else
   W=${V}_obsd.tar.gz
  fi
 fi
 wget $L/$W
 tar -xf $W
fi

# ----------------------------------------------------------------------
if [ "$OS" = "Linux" ] && [ "$D" = "/usr" ]; then
 cd $V
 bin/jconsole -js "exit 0[echo JVERSION"
 bin/jconsole -js "install 'system $P $A'"
 if [ -f "/etc/alternatives/ijconsole" ]; then
  update-alternatives --remove-all ijconsole
 fi
 bin/install-usr.sh
else
 mkdir -p $D/$V
 cp -r $M/$V/* $D/$V
 cd $D/$V
 bin/jconsole -js "exit 0[echo JVERSION"
 bin/jconsole -js "install 'system $P $A $S'"
fi
