#include <QApplication>
#include <QDebug>
#include <QSysInfo>
#include <QFont>
#include <QDateTime>

using namespace std;

#ifdef QT_OS_ANDROID
#include <jni.h>
extern int state_run (int,char **,QApplication *,QString);
extern "C" void javaOnLoad(JavaVM * vm, JNIEnv * env);

#else
#include <QLibrary>

typedef int (*Run)(int,char **,QApplication *,QString);
#endif

int main(int argc, char *argv[])
{
#ifdef __MACH__
  if ( QSysInfo::MacintoshVersion > QSysInfo::MV_10_8 ) {
    // fix Mac OS X 10.9 (mavericks) font issue
    // https://bugreports.qt-project.org/browse/QTBUG-32789
    QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
  }
#endif

  qsrand(QDateTime::currentMSecsSinceEpoch());
  QApplication app(argc, argv);

#ifdef QT_OS_ANDROID
  return state_run(argc, argv, &app, QCoreApplication::applicationFilePath());
#else
#ifdef _WIN32
#ifndef FHS
  QString s=QCoreApplication::applicationDirPath() + "/jqt";
#else
  QString s= "jqt";
#endif
#else
#ifndef FHS
  QString s=QCoreApplication::applicationDirPath() + "/libjqt";
#else
  QString s= "libjqt";
#endif
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

#ifdef QT_OS_ANDROID
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*)
{
  JNIEnv *jnienv;
  if (vm->GetEnv(reinterpret_cast<void**>(&jnienv), JNI_VERSION_1_6) != JNI_OK) {
    qCritical() << "JNI_OnLoad GetEnv Failed";
    return -1;
  }
  javaOnLoad(vm, jnienv);

  return JNI_VERSION_1_6;
}
#endif
