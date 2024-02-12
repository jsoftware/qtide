#!/bin/sh
set -e 

cd "$(dirname "$0")"
p=`pwd`

rm -rf /tmp/jx
mkdir -p /tmp/jx
cd /tmp/jx
curl -L -O https://github.com/jsoftware/jsource/archive/master.tar.gz
tar -xzf master.tar.gz
curl -L -O https://github.com/jsoftware/jsource/releases/download/build/wasm32.zip
unzip wasm32.zip

cd $p

mkdir -p wasm/j32
cp /tmp/jx/j32/libj.a wasm/j32/.
rm -f libgmp.a
wget https://www.jsoftware.com/download/jengine/mpir/linux/wasm32/libgmp.a
mkdir -p wasm/mpir
mv libgmp.a wasm/mpir

rm -rf test
cp -r /tmp/jx/jsource-master/test .

find test -type f -print > jtst.qrc
sed -i "" 's/^/<file>/' jtst.qrc
sed -i "" 's/$/<\/file>/' jtst.qrc

tee -a jtst.qrc <<EOF
</qresource>
</RCC>
EOF

tee hd <<EOF
<!DOCTYPE RCC><RCC version="1.0">
<qresource prefix="jtest">
EOF

cat hd jtst.qrc > test.qrc 

rm -f hd jtst.qrc


