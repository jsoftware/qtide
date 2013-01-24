#include <QApplication>
#include <QDebug>

using namespace std;

#ifdef ANDROID
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

#ifdef ANDROID
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
  qDebug() << "could not resolve: state_run";

  return -1;
#endif
}

#ifdef ANDROID
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*)
{
  JNIEnv *jnienv;
  jnivm = vm;
  if (vm->GetEnv(reinterpret_cast<void**>(&jnienv), JNI_VERSION_1_6) != JNI_OK) {
    qCritical() << "JNI_OnLoad GetEnv Failed";
    return -1;
  }

  qDebug() << "JNI_OnLoad vm " << QString::number((long)vm);
  qDebug() << "JNI_OnLoad env " << QString::number((long)jnienv);
  qtapp=jnienv->FindClass("com/jsoftware/android/qtide/QtApplication");
  qtact=jnienv->FindClass("com/jsoftware/android/qtide/QtActivity");
  qDebug() << "com/jsoftware/android/qtide/QtApplication jclass " << QString::number((long)qtapp);
  qDebug() << "com/jsoftware/android/qtide/QtActivity jclass " << QString::number((long)qtact);

  return JNI_VERSION_1_6;
}
#endif
