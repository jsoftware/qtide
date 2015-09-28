#include <QDebug>
#include <QSysInfo>
#include <QFile>
#include <QFont>
#include <QLibrary>

using namespace std;

typedef int (*Run)(int,char **,char *,bool,bool,void *,void *,void **,void **);
static Run state_run;
extern "C" char * jepath1(char* arg);

#if defined(_WIN32) && defined(JQTOLECOM)
extern int initexeserver();
extern int reg(int set, char* keys);
#endif

void *hjdll;
void *pjst;

extern "C" int staterun(int arg, char *lib, int fhs)
{
  if (state_run && arg<0)
    return state_run(arg,0,lib,(!!fhs),false,0,(void *)-1,0,0);
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
  bool embedding=false;
#ifdef _WIN32
#if defined(JQTOLECOM)
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
    embedding=true;
    argc= 1;
    strcpy(argv[1],"-jprofile");  // no buffer overflow
  }
#endif
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
    state_run(argc, argv, lib->fileName().toUtf8().data(),fhs,(!embedding),0,(void *)-1, &hjdll, &pjst);
#if defined(_WIN32) && defined(JQTOLECOM)
    if (embedding)
      if (!initexeserver())
        return -1;
#endif
    return staterun(-1,0,0);
  }

  qDebug() << lib->fileName();
  qDebug() << "could not resolve: state_run:\n\n" + lib->errorString();

  return -1;
}

