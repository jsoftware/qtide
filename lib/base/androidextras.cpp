#include <QDebug>
#include "androidextras.h"

float DM_density=1.0;
float DM_scaledDensity=1.0;
float DM_xdpi=160.0;
float DM_ydpi=160.0;
int DM_densityDpi=160;
int DM_heightPixels=160;
int DM_rotation;
int DM_widthPixels=160;

static QAndroidJniObject getMainActivity()
{
  return QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
}

void * getqtactivity()
{
  return getMainActivity().object<jobject>();
}

void android_exec_host(char* action, char* uri, char* mime, int flag)
{
  QAndroidJniEnvironment env;
  QAndroidJniObject intent("android/content/Intent", "(Ljava/lang/String;)V", QAndroidJniObject::fromString(QString::fromUtf8(action)).object<jstring>());
  if (env->ExceptionCheck()) {
    // Handle exception here.
    env->ExceptionDescribe();
    qDebug() << "androidextras jni error";
    env->ExceptionClear();
    return;
  }
  if (!intent.isValid()) return;
  if (uri && strlen(uri)) {
    QAndroidJniObject objuri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", QAndroidJniObject::fromString(QString::fromUtf8(uri)).object<jstring>());
    if (mime && strlen(mime))
      intent.callObjectMethod("setDataAndType", "(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;", objuri.object<jobject>(), QAndroidJniObject::fromString(QString::fromUtf8(mime)).object<jstring>());
    else intent.callObjectMethod("setData", "(Landroid/net/Uri;)Landroid/content/Intent;", objuri.object<jobject>());
  } else if (mime && strlen(mime))
    intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QAndroidJniObject::fromString(QString::fromUtf8(mime)).object<jstring>());
  if (flag)
    intent.callObjectMethod("setFlags", "(I)Landroid/content/Intent;", flag);
  QtAndroid::startActivity(intent,0);
  if (env->ExceptionCheck()) {
    // Handle exception here.
    env->ExceptionDescribe();
    qDebug() << "androidextras jni error";
    env->ExceptionClear();
    return;
  }
}

// return 8 double : rotation, density, densityDpi, heightPixels, scaledDensity, widthPixels, xdpi, ydpi
void android_getdisplaymetrics(double * dmetrics)
{
  QAndroidJniEnvironment env;
  QAndroidJniObject act = getMainActivity();
  if (env->ExceptionCheck()) {
    // Handle exception here.
    env->ExceptionDescribe();
    qDebug() << "androidextras jni error";
    env->ExceptionClear();
    return;
  }
  if (!act.isValid()) return;
  QAndroidJniObject wm = act.callObjectMethod("getWindowManager","()Landroid/view/WindowManager;");
  QAndroidJniObject ds = wm.callObjectMethod("getDefaultDisplay", "()Landroid/view/Display;");
  QAndroidJniObject dm("android/util/DisplayMetrics");

// the following line crashed, workaround with direct jni calls
//  ds.callObjectMethod("getMetrics", "(Landroid/util/DisplayMetrics;)V",dm.object<jobject>());
  jobject ods = ds.object<jobject>();
  jobject odm = dm.object<jobject>();
  jclass odsclass = env->GetObjectClass(ods);
  jmethodID mid = env->GetMethodID(odsclass, "getMetrics", "(Landroid/util/DisplayMetrics;)V");
  env->CallVoidMethod(ods, mid, odm);
  env->DeleteLocalRef(odsclass);

  DM_density = dm.getField<jfloat>("density");
  DM_scaledDensity = dm.getField<jfloat>("scaledDensity");
  DM_densityDpi = dm.getField<jint>("densityDpi");
  DM_heightPixels = dm.getField<jint>("heightPixels");
  DM_widthPixels = dm.getField<jint>("widthPixels");
  DM_xdpi = dm.getField<jfloat>("xdpi");
  DM_ydpi = dm.getField<jfloat>("ydpi");
  if (((DM_xdpi>DM_ydpi)?DM_xdpi:DM_ydpi)<0.5*DM_densityDpi) DM_xdpi = DM_ydpi = (float)DM_densityDpi;  // workaround android bug
  DM_rotation = ds.getField<jint>("getOrientation");
  if (env->ExceptionCheck()) {
    // Handle exception here.
    env->ExceptionDescribe();
    qDebug() << "androidextras jni error";
    env->ExceptionClear();
  }
  if (dmetrics) {
    *dmetrics++=DM_rotation;
    *dmetrics++=DM_density;
    *dmetrics++=DM_densityDpi;
    *dmetrics++=DM_heightPixels;
    *dmetrics++=DM_scaledDensity;
    *dmetrics++=DM_widthPixels;
    *dmetrics++=DM_xdpi;
    *dmetrics++=DM_ydpi;
  }
}
