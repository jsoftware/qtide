#!/bin/sh
set -e 

cd "$(dirname "$0")"
p=`pwd`

jver="j9.7"

if ( [ "`uname`" = "Linux" ] )  && ( [ "`uname -m`" = "armv6l" ] ); then
  jf="${jver}_raspi32.tar.gz"
elif ( [ "`uname`" = "Linux" ] )  && ( [ "`uname -m`" = "aarch64" ]  || [ "`uname -m`" = "arm64" ] ); then
  jf="${jver}_raspi64.tar.gz"
elif [ "`uname`" = "Darwin" ]; then
  jf="${jver}_mac64.zip"
elif [ "`uname`" = "OpenBSD" ]; then
  jf="${jver}_obsd64.tar.gz"
elif [ "`uname`" = "FreeBSD" ]; then
  jf="${jver}_fbsd64.tar.gz"
else
  jf="${jver}_linux64.tar.gz"
fi

rm -rf /tmp/jx
mkdir -p /tmp/jx
cd /tmp/jx
wget https://www.jsoftware.com/download/$jver/install/${jf}
unzip $jf
tee -a dn.ijs <<EOF
load 'pacman'

REQ=: <;._2 [ 0 : 0
demos/qtdemo
demos/wd
labs/labs
)

'update'jpkg''
'install'jpkg REQ
'upgrade'jpkg ''
exit 0
EOF

${jver}/bin/jconsole dn.ijs

cd $p

rm -rf jlibrary
mkdir -p jlibrary/bin
cp /tmp/jx/${jver}/bin/profile.ijs jlibrary/bin/.
cp -r /tmp/jx/${jver}/addons jlibrary/.
cp -r /tmp/jx/${jver}/system jlibrary/.

find jlibrary -type f -print > jlib.qrc
sed -i "" 's/^/<file>/' jlib.qrc
sed -i "" 's/$/<\/file>/' jlib.qrc

tee -a jlib.qrc <<EOF
</qresource>
</RCC>
EOF

tee hd <<EOF
<!DOCTYPE RCC><RCC version="1.0">
<qresource prefix="j">
EOF

cat hd jlib.qrc > jlibrary.qrc 

rm -f hd jlib.qrc


