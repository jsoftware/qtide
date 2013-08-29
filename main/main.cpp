#include <QApplication>
#include <QDebug>

using namespace std;

#ifdef Q_OS_ANDROID
#include <jni.h>
extern int state_run (int,char **,QApplication *,QString,void *,void *,void *);

static JavaVM *jnivm=0;
static jclass qtapp=0;
static jclass qtact=0;
#else
#include <QLibrary>

typedef int (*Run)(int,char **,QApplication *,QString);
#endif

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

#ifdef Q_OS_ANDROID
  return state_run(argc, argv, &app, QCoreApplication::applicationFilePath(), (void *)jnivm, (void *)qtapp, (void *)qtact);
#else
#ifdef _WIN32
  QString s=QCoreApplication::applicationDirPath() + "/jqt";
#else
  QString s=QCoreApplication::applicationDirPath() + "/libjqt";
#endif
  QLibrary *lib=new QLibrary(s);
  Run state_run=(Run) lib->resolve("state_run");
  if (state_run)
    return state_run(argc, argv, &app, lib->fileName());

  qDebug() << lib->fileName();
  qDebug() << "could not resolve: state_run:\n\n" + lib->errorString();

  return -1;
#endif
}

#ifdef Q_OS_ANDROID
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*)
{
  JNIEnv *jnienv;
  if (vm->GetEnv(reinterpret_cast<void**>(&jnienv), JNI_VERSION_1_6) != JNI_OK) {
    qCritical() << "JNI_OnLoad GetEnv Failed";
    return -1;
  }
  jnivm = vm;
  jclass ap,ac;
  qDebug() << "JNI_OnLoad vm " << QString::number((long)vm);
  qDebug() << "JNI_OnLoad env " << QString::number((long)jnienv);
  ap=jnienv->FindClass("org/qtproject/qt5/android/bindings/QtApplication");
  ac=jnienv->FindClass("org/qtproject/qt5/android/bindings/QtActivity");
  qtapp=(jclass)jnienv->NewGlobalRef(ap);
  qtact=(jclass)jnienv->NewGlobalRef(ac);
  jnienv->DeleteLocalRef(ap);
  jnienv->DeleteLocalRef(ac);
  qDebug() << "org/qtproject/qt5/android/bindings/QtApplication jclass " << QString::number((long)qtapp);
  qDebug() << "org/qtproject/qt5/android/bindings/QtActivity jclass " << QString::number((long)qtact);

  return JNI_VERSION_1_6;
}
#endif
