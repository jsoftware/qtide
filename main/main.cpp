#include <QtGlobal>
#include <QDebug>
#include <QSysInfo>
#include <QFile>
#include <QFont>
#include <QLibrary>
#include <QString>

// using namespace std;

#ifndef _WIN32
#include <unistd.h>
#include <sys/resource.h>
#include <stdint.h>
#endif
#include <locale.h>
#ifdef __APPLE__
#include <xlocale.h>
#endif

#ifdef Q_OS_ANDROID
#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>

static int start_logger(void);
static int pfd[2];
static pthread_t thr;
static const char *logtag = "libj";
#endif

#ifdef JQTAMALGAM
extern "C" int state_run(int,char **,const char *,bool,int,void **,void **,uintptr_t);
#else
typedef int (*Run)(int,char **,const char *,bool,int,void **,void **,uintptr_t);
static Run state_run;
#endif
extern "C" char * jepath1(char* arg);

extern int initexeserver();
extern int reg(int set, char* keys);

#ifdef JQTAMALGAM
extern void *hjdll;
#else
#ifdef Q_OS_ANDROID
extern void *hjdll;
#else
void *hjdll;
#endif
#endif
void *pjst;
static uintptr_t cstackinit;

extern "C" int staterun(int argc, char *arg1, int arg2)
{
#ifdef JQTAMALGAM
  if (argc<0)
#else
  if (state_run && argc<0)
#endif
    return state_run(argc,0,arg1,false,arg2,0,0,cstackinit);
  else
    return 0;
}

#ifdef JQTAMALGAM
extern char *jqtver;
#else
#ifdef Q_OS_ANDROID
extern char *jqtver;
#else
const char *jqtver=JQTVERSION;
#endif
#endif

int main(int argc, char *argv[])
{
  double y;
  cstackinit=(uintptr_t)&y;
  Q_UNUSED(jqtver);
#if !(defined(_WIN32))
  locale_t loc=0;
  if ((loc = newlocale(LC_ALL_MASK, "", (locale_t)0 )))
    if ((loc = newlocale(LC_NUMERIC_MASK, "C", loc ))) uselocale(loc);
#else
  setlocale(LC_ALL, "");
  setlocale(LC_NUMERIC,"C");
#endif
#if defined(__APPLE__)
#if !defined(QT50)
  if ( QSysInfo::MacintoshVersion > QSysInfo::MV_10_8 ) {
    // fix Mac OS X 10.9 (mavericks) font issue
    // https://bugreports.qt-project.org/browse/QTBUG-32789
    QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
  }
#endif
#if !defined(Q_OS_IOS)
  // Disable (unsupported) macOS Sierra tab bar functionality if necessary.
  extern void disableWindowTabbing();
  disableWindowTabbing();
#endif
#endif

#if defined(Q_OS_ANDROID)
  start_logger();
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
#else
  QString s= QString::fromUtf8(path);
#if defined(__APPLE__)
  if(s.startsWith("/usr/local/bin")||s.startsWith("/opt/homebrew/bin")) {
    s= s+QString("/../lib/")+QString("libjqt.")+QString(APP_VERSION)+QString(".dylib");
    fhs = true;
  }
#else
  if(s.startsWith("/usr/local/bin")||s.startsWith("/usr/bin")) {
    s= QString("libjqt.so")+"."+QString(APP_VERSION);
    fhs = true;
  }
#endif
  else s= s+ "/libjqt";
#endif
  if(!fhs) {
#ifdef _WIN32
    s=s+".dll";
#else
#if defined(__APPLE__)
    s=s+".dylib";
#else
    s=s+".so";
#endif
#endif
  }
#ifdef JQTAMALGAM
  if (1) {
    state_run(argc, argv, (char*)"",fhs,(embedding)?0:1, &hjdll, &pjst, cstackinit);
#else
  QLibrary *lib=new QLibrary(s);
  state_run=(Run) lib->resolve("state_run");
  if (state_run) {
    state_run(argc, argv, lib->fileName().toUtf8().data(),fhs,(embedding)?0:1, &hjdll, &pjst, cstackinit);
#endif
#if defined(_WIN32)
    if (embedding)
      if (!initexeserver())
        return -1;
    return staterun(-1,0,0);
#else
    return state_run(-1,0,0,false,0,0,0,cstackinit);
#endif
  }

#ifndef JQTAMALGAM
  qDebug() << lib->fileName();
  qDebug() << "could not resolve: state_run:\n\n" + lib->errorString();
#endif

  return -1;
}

#ifdef Q_OS_ANDROID
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*)
{
  JNIEnv *jnienv;
  if (vm->GetEnv(reinterpret_cast<void**>(&jnienv), JNI_VERSION_1_6) != JNI_OK) {
    qCritical() << "JNI_OnLoad GetEnv Failed";
    return -1;
  }
//  javaOnLoad(vm, jnienv);

  return JNI_VERSION_1_6;
}

static void *thread_func(void*)
{
  ssize_t rdsz;
  char buf[128];
  while((rdsz = read(pfd[0], buf, sizeof buf - 1)) > 0) {
    if(buf[rdsz - 1] == '\n') --rdsz;
    buf[rdsz] = 0;  /* add null-terminator */
    __android_log_write(ANDROID_LOG_DEBUG, logtag, buf);
  }
  return 0;
}

static int start_logger(void)
{
  /* make stdout and stderr unbuffered */
  setvbuf(stdout, 0, _IONBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);

  /* create the pipe and redirect stdout and stderr */
  pipe(pfd);
  dup2(pfd[1], 1);
  dup2(pfd[1], 2);

  /* spawn the logging thread */
  if(pthread_create(&thr, 0, thread_func, 0) == -1)
    return -1;
  pthread_detach(thr);
  return 0;
}

#endif
