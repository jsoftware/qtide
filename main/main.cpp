#include <QDebug>
#include <QSysInfo>
#include <QFile>
#include <QFont>
#include <QLibrary>

using namespace std;

typedef int (*Run)(int,char **,char *,bool,void *,void *);
extern "C" char * jepath1(char* arg);

int main(int argc, char *argv[])
{
#if defined(__MACH__) && !defined(QT50)
  if ( QSysInfo::MacintoshVersion > QSysInfo::MV_10_8 ) {
    // fix Mac OS X 10.9 (mavericks) font issue
    // https://bugreports.qt-project.org/browse/QTBUG-32789
    QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
  }
#endif
  char *path=jepath1(argv[0]);     // get path to JFE folder

  bool fhs = false;
#ifdef _WIN32
  QString s= QString::fromUtf8(path)+ "/jqt";
  if(!(QFile(s.append(".dll"))).exists()) {
    s= "jqt.dll";
    fhs = true;
  }
#else
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
    return state_run(argc, argv, lib->fileName().toUtf8().data(),fhs,0,(void *)-1);

  qDebug() << lib->fileName();
  qDebug() << "could not resolve: state_run:\n\n" + lib->errorString();

  return -1;
}

