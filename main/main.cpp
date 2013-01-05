#include <QApplication>
#include <QLibrary>

#include <QDebug>

using namespace std;

typedef int (*Run)(int,char **,QApplication *,QString);

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

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
}

