#include <QApplication>
#include <QDebug>
#include <QSysInfo>
#include <QFile>
#include <QFont>

using namespace std;

#ifdef QT_OS_ANDROID
#include <jni.h>
extern "C" int state_run (int,char **,char *,bool,void *);
extern "C" void javaOnLoad(JavaVM * vm, JNIEnv * env);

#else
#include <QLibrary>

typedef int (*Run)(int,char **,char *,bool,bool);

extern "C" char * jepath1(char* arg);
#endif

int main(int argc, char *argv[])
{
#if defined(__MACH__) && !defined(QT50)
  if ( QSysInfo::MacintoshVersion > QSysInfo::MV_10_8 ) {
    // fix Mac OS X 10.9 (mavericks) font issue
    // https://bugreports.qt-project.org/browse/QTBUG-32789
    QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
  }
#endif
#ifdef QT_OS_ANDROID
  QApplication app(argc, argv);
#else
  char *path=jepath1(argv[0]);     // get path to JFE folder
#endif

  bool fhs = false;
#ifdef QT_OS_ANDROID
  fhs = true;
  return state_run(argc, argv, QCoreApplication::applicationFilePath().toUtf8().data(),fhs,0);
#else
#ifdef _WIN32
//  QString s=QCoreApplication::applicationDirPath() + "/jqt";
  QString s= QString::fromUtf8(path)+ "/jqt";
  if(!(QFile(s.append(".dll"))).exists()) {
    s= "jqt.dll";
    fhs = true;
  }
#else
//  QString s=QCoreApplication::applicationDirPath() + "/libjqt";
  QString s= QString::fromUtf8(path)+ "/libjqt";
#if defined(__MACH__)
  if(!(QFile(s.append(".dylib"))).exists()) {
#else
  if(!(QFile(s.append(".so"))).exists()) {
#endif
#if defined(__MACH__)
    s= "libjqt.dylib";
#else
    s= "libjqt.so";
#endif
    fhs = true;
  }
#endif
  QLibrary *lib=new QLibrary(s);
  Run state_run=(Run) lib->resolve("state_run");
  if (state_run)
    return state_run(argc, argv, lib->fileName().toUtf8().data(),fhs,0);

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
