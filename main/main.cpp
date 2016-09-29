#include <QDebug>
#include <QSysInfo>
#include <QFile>
#include <QFont>
#include <QLibrary>
#include <QString>

using namespace std;

typedef int (*Run)(int,char **,char *,bool,int,void *,void *,void **,void **);
static Run state_run;
extern "C" char * jepath1(char* arg);

extern int initexeserver();
extern int reg(int set, char* keys);

void *hjdll;
void *pjst;

extern "C" int staterun(int argc, char *arg1, int arg2)
{
  if (state_run && argc<0)
    return state_run(argc,0,arg1,false,arg2,0,0,0,0);
  else
    return 0;
}

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
  int embedding=0;
#ifdef _WIN32
  int regn=-1;
  if (argc>1 && (!strcmp(argv[1],"-regserver") || !strcmp(argv[1],"/regserver")))
    regn=1;
  else if (argc>1 && (!strcmp(argv[1],"-unregserver") || !strcmp(argv[1],"/unregserver")))
    regn=0;
  if (regn>=0) {
    char keys[2000];
    reg(regn, keys);
    exit(0);
  }
  if (argc>1 && !strcmp(argv[1],"-Embedding")) {
    embedding=1;
    argc= 2;
    strcpy(argv[1],"-jprofile");  // no buffer overflow
  }
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
  state_run=(Run) lib->resolve("state_run");
  if (state_run) {
    state_run(argc, argv, lib->fileName().toUtf8().data(),fhs,(embedding)?0:1,0,(void *)-1, &hjdll, &pjst);
#if defined(_WIN32)
    if (embedding)
      if (!initexeserver())
        return -1;
    return staterun(-1,0,0);
#else
    return state_run(-1,0,0,false,0,0,0,0,0);
#endif
  }

  qDebug() << lib->fileName();
  qDebug() << "could not resolve: state_run:\n\n" + lib->errorString();

  return -1;
}

