#ifndef ANDROIDEXTRAS_H
#define ANDROIDEXTRAS_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#include <QtAndroid>
#include <QAndroidJniObject>

using QtJniObject = QAndroidJniObject;

QtJniObject qtAndroidContext();

int qtAndroidSdkVersion();

void qt5RunOnAndroidMainThread(const QtAndroid::Runnable &runnable);

#else

#include <QJniEnvironment>
#include <QCoreApplication>
#include <QJniObject>

using QtJniObject = QJniObject;

QtJniObject qtAndroidContext();

int qtAndroidSdkVersion();

void qt5RunOnAndroidMainThread(const std::function<void()> &runnable);

#endif // #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

extern int DM_rotation;
extern float DM_density;
extern float DM_scaledDensity;
extern int DM_densityDpi;
extern float DM_xdpi;
extern float DM_ydpi;
extern int DM_widthPixels;
extern int DM_heightPixels;
extern int DM_widthPixelsFull;
extern int DM_heightPixelsFull;

extern "C" {
  void * getqtactivity();
  void android_exec_host(char* action, char* uri, char* mime, int flag);
  void android_getdisplaymetrics();
}

#endif
