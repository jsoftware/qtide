#include <QDebug>
#include <QApplication>
#include <QScreen>
#include "androidextras.h"

extern QApplication *app;

int DM_rotation=0;
float DM_density=0.0;
float DM_scaledDensity;
int DM_densityDpi;
float DM_xdpi;
float DM_ydpi;
int DM_widthPixels;
int DM_heightPixels;
int DM_widthPixelsFull;
int DM_heightPixelsFull;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

QtJniObject qtAndroidContext()
{
  auto result = QtAndroid::androidActivity();
  if (result.isValid())
    return result;
  return QtAndroid::androidService();
}

int qtAndroidSdkVersion()
{
  return QtAndroid::androidSdkVersion();
}

void qt5RunOnAndroidMainThread(const QtAndroid::Runnable &runnable)
{
  QtAndroid::runOnAndroidThread(runnable);
}

#else

QtJniObject qtAndroidContext()
{
  return QJniObject(QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context());
}

int qtAndroidSdkVersion()
{
  return QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->sdkVersion();
}

void qt5RunOnAndroidMainThread(const std::function<void()> &runnable)
{
  QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->runOnAndroidMainThread([runnable]() {
    runnable();
    return QVariant();
  });
}

#endif // #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

void * getqtactivity()
{
  return qtAndroidContext().object<jobject>();
}

void android_exec_host(char* action, char* uri, char* mime, int flag)
{
  qDebug() << "android_exec_host" ;
  QJniEnvironment env;
  QJniObject intent("android/content/Intent", "(Ljava/lang/String;)V", QJniObject::fromString(QString::fromUtf8(action)).object<jstring>());
  if (env->ExceptionCheck()) {
    // Handle exception here.
    env->ExceptionDescribe();
    qDebug() << "androidextras jni error";
    env->ExceptionClear();
    return;
  }
  if (!intent.isValid()) return;
  if (uri && strlen(uri)) {
    QJniObject objuri = QJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", QJniObject::fromString(QString::fromUtf8(uri)).object<jstring>());
    if (mime && strlen(mime))
      intent.callObjectMethod("setDataAndType", "(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;", objuri.object<jobject>(), QJniObject::fromString(QString::fromUtf8(mime)).object<jstring>());
    else intent.callObjectMethod("setData", "(Landroid/net/Uri;)Landroid/content/Intent;", objuri.object<jobject>());
  } else if (mime && strlen(mime))
    intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QJniObject::fromString(QString::fromUtf8(mime)).object<jstring>());
  if (flag)
    intent.callObjectMethod("setFlags", "(I)Landroid/content/Intent;", flag);
//  QtAndroid::startActivity(intent,0);
  if (env->ExceptionCheck()) {
    // Handle exception here.
    env->ExceptionDescribe();
    qDebug() << "androidextras jni error";
    env->ExceptionClear();
    return;
  }
}

void android_getdisplaymetrics()
{
  if(0.0!=DM_density) return;
  qDebug() << "android_getdisplaymetrics" ;

  QJniObject act = qtAndroidContext();
  if (!act.isValid()) return;
  QJniObject resources = act.callObjectMethod("getResources","()Landroid/content/res/Resources;");
  QJniObject dm = resources.callObjectMethod("getDisplayMetrics","()Landroid/util/DisplayMetrics;");

  DM_rotation = act.callMethod<jint>("getRequestedOrientation");
  DM_density = dm.getField<jfloat>("density");
  DM_scaledDensity = dm.getField<jfloat>("scaledDensity");
  DM_densityDpi = dm.getField<jint>("densityDpi");
  DM_xdpi = dm.getField<jfloat>("xdpi");
  DM_ydpi = dm.getField<jfloat>("ydpi");
  if (((DM_xdpi>DM_ydpi)?DM_xdpi:DM_ydpi)<0.5*DM_densityDpi) DM_xdpi = DM_ydpi = (float)DM_densityDpi;  // workaround android bug
  DM_heightPixels = dm.getField<jint>("heightPixels");
  DM_widthPixels = dm.getField<jint>("widthPixels");
  DM_heightPixelsFull = 10*roundf(0.1*app->primaryScreen()->virtualSize().height()*DM_ydpi/160.0);
  DM_widthPixelsFull =  10*roundf(0.1*app->primaryScreen()->virtualSize().width()*DM_xdpi/160.0);

  qDebug() << "DM_rotation" << DM_rotation;
  qDebug() << "DM_density" << DM_density;
  qDebug() << "DM_scaledDensity" << DM_scaledDensity;
  qDebug() << "DM_densityDpi" << DM_densityDpi;
  qDebug() << "DM_xdpi" << DM_xdpi;
  qDebug() << "DM_ydpi" << DM_ydpi;
  qDebug() << "DM_widthPixels" << DM_widthPixels;
  qDebug() << "DM_heightPixels" << DM_heightPixels;
  qDebug() << "DM_widthPixelsFull" << DM_widthPixelsFull;
  qDebug() << "DM_heightPixelsFull" << DM_heightPixelsFull;
}
