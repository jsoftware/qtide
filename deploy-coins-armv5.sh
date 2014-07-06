#!/bin/bash

N=coins
A=armv5
S=$(dirname "$0")

cd $S

QMAKE_TARGET=libjqt.so
INSTALL_ROOT=android-build
ANDROID_PLATFORM=android-10
PLATFORM_ID=14  # 14 -> api 10,  22-> api 12

rm -rf ${INSTALL_ROOT}
test -d ${INSTALL_ROOT}/libs/armeabi/ || mkdir -p ${INSTALL_ROOT}/libs/armeabi/
install "bin/android-arm/release/${QMAKE_TARGET}" "${INSTALL_ROOT}/libs/armeabi/${QMAKE_TARGET}"
$HOME/Qt/5.3.0/5.3/android_armv5/bin/androiddeployqt --ant true --input $N-$A.json --output ${INSTALL_ROOT} --deployment bundled --android-platform ${ANDROID_PLATFORM} --release -verbose
android update project --path ${INSTALL_ROOT} --target ${PLATFORM_ID} --name $N
sed -i -e 's/super\.onCreate(savedInstanceState);/super\.onCreate(savedInstanceState);QtApplication\.m_activity = this;/' ${INSTALL_ROOT}/src/org/qtproject/qt5/android/bindings/QtActivity.java 
sed -i -e 's/public final static String QtTAG = "Qt";/public final static String QtTAG = "Qt";public static QtActivity m_activity = null;/' ${INSTALL_ROOT}/src/org/qtproject/qt5/android/bindings/QtApplication.java
cd ${INSTALL_ROOT} && ant release || exit 1
