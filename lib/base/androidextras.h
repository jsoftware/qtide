#ifndef ANDROIDEXTRAS_H
#define ANDROIDEXTRAS_H

#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>

#include <jni.h>

extern float DM_density;
extern float DM_scaledDensity;
extern float DM_xdpi;
extern float DM_ydpi;
extern int DM_densityDpi;
extern int DM_heightPixels;
extern int DM_rotation;
extern int DM_widthPixels;

extern "C" {
  void android_exec_host(char* action, char* uri, char* mime, int flag);
  void android_getdisplaymetrics(double *dmetrics);
}

#endif
