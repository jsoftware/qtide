#include <QtGlobal>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QStandardPaths>

#include <csignal>
#include <assert.h>
#include <errno.h>
#include <stdint.h>

#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef __MACH__
#include <mach-o/dyld.h>    // NSGetExecutablePath
#endif
#if defined(Q_OS_IOS)
#include "../../main/redminedevicehelper.h"
#endif

#include "base.h"
#include "jsvr.h"
#include "util.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

#ifdef Q_OS_ANDROID
#include <QDir>
#include <sys/stat.h>
extern QString AndroidPackage;
#endif
void * hjdll=0;

static char pathetcpx[PLEN];
#if !defined(Q_OS_IOS) && !defined(Q_OS_WASM) && !defined(Q_OS_ANDROID)
static char pathexec0[PLEN];
static char pathexec[PLEN];
#endif

// using namespace std;

int _stdcall JDo(JS jt,C*);                   /* run sentence */
void _stdcall JInterrupt(JS jt);              /* interrupt */
C* _stdcall JGetLocale(JS jt);                /* get locale */
JS _stdcall JInit();                          /* init instance */
int _stdcall JFree(JS jt);                    /* free instance */
A _stdcall JGetA(JS jt,I n,C* name);          /* get 3!:1 from name */
I _stdcall JSetA(JS jt,I n,C* name,I x,C* d); /* name=:3!:2 data */
void _stdcall JSM(JS jt, void*callbacks[]);  /* set callbacks */

A jega(I t, I n, I r, I*s);
char* jegetlocale();
extern "C" Dllexport void* jehjdll();

char **adadbreak;
char inputline[BUFLEN+1];
JS jt=0;

static char path[PLEN];
static char pathdll[PLEN];

static JDoType jdo;
static JInterruptType jinterrupt;
static JFreeType jfree;
static JgaType jga;
static JGetAType jgeta;
static JGetLocaleType jgetlocale;
static JSetAType jseta;

extern QString LibName;
bool FHS=false;
bool standAlone=false;
QString documentsPath;
QString homePath;
QString installPath;

// ---------------------------------------------------------------------
void addargv(int argc, char* argv[], C* d)
{
  C *p,*q;
  I i;

  p=d+strlen(d);
  for (i=0; i<argc; ++i) {
    if (sizeof(inputline)<(100+strlen(d)+2*strlen(argv[i]))) exit(100);
    if (1==argc) {
      *p++=',';
      *p++='<';
    }
    if (i)*p++=';';
    *p++='\'';
    q=argv[i];
    while (*q) {
      *p++=*q++;
      if ('\''==*(p-1))*p++='\'';
    }
    *p++='\'';
  }
  *p=0;
}

// ---------------------------------------------------------------------
int jedo(char* sentence)
{
  if (!jt) return 0;
#ifdef DEBUG_JDO
  qDebug() << "jsvr jedo " + QString(sentence);
#endif
  return jdo(jt,sentence);
}

// ---------------------------------------------------------------------
void jefree()
{
  if (jt) jfree(jt);
  jt=0;
}

// ---------------------------------------------------------------------
char* jegetlocale()
{
  if (!jt) return (char *)"base";
  return jgetlocale(jt);
}

// ---------------------------------------------------------------------
A jega(I t, I n, I r, I*s)
{
  if (!jt) return 0;
  return jga(jt,t,n,r,s);
}

// ---------------------------------------------------------------------
void* jehjdll()
{
  return hjdll;
}

// ---------------------------------------------------------------------
// load JE, Jinit, getprocaddresses, JSM
JS jeload(void* callbacks)
{
#if defined(Q_OS_IOS) || defined(Q_OS_WASM) || defined(Q_OS_ANDROID)
  jt=JInit();
  if (!jt) return 0;
  JSM(jt,(void**)callbacks);
  jdo=JDo;
  jinterrupt=JInterrupt;
  jfree=JFree;
  jga=Jga;
  jgeta=JGetA;
  jgetlocale=JGetLocale;
  jseta=JSetA;
  return jt;
#elif defined(_WIN32)
  WCHAR wpath[PLEN];
  MultiByteToWideChar(CP_UTF8,0,pathdll,1+(int)strlen(pathdll),wpath,PLEN);
  hjdll=LoadLibraryW(wpath);
  if (!hjdll)return 0;
  jt=((JInitType)GETPROCADDRESS((HMODULE)hjdll,"JInit"))();
  if (!jt) return 0;
  ((JSMType)GETPROCADDRESS((HMODULE)hjdll,"JSM"))(jt,(void**)callbacks);
  jdo=(JDoType)GETPROCADDRESS((HMODULE)hjdll,"JDo");
  jinterrupt=(JInterruptType)GETPROCADDRESS((HMODULE)hjdll,"JInterrupt");
  jfree=(JFreeType)GETPROCADDRESS((HMODULE)hjdll,"JFree");
  jga=(JgaType)GETPROCADDRESS((HMODULE)hjdll,"Jga");
  jgeta=(JGetAType)GETPROCADDRESS((HMODULE)hjdll,"JGetA");
  jgetlocale=(JGetLocaleType)GETPROCADDRESS((HMODULE)hjdll,"JGetLocale");
  jseta=(JSetAType)GETPROCADDRESS((HMODULE)hjdll,"JSetA");
  return jt;
#else
  hjdll=dlopen(pathdll,RTLD_LAZY);
  if (!hjdll)return 0;
  jt=((JInitType)GETPROCADDRESS(hjdll,"JInit"))();
  if (!jt) return 0;
  ((JSMType)GETPROCADDRESS(hjdll,"JSM"))(jt,(void**)callbacks);
  jdo=(JDoType)GETPROCADDRESS(hjdll,"JDo");
  jinterrupt=(JInterruptType)GETPROCADDRESS(hjdll,"JInterrupt");
  jfree=(JFreeType)GETPROCADDRESS(hjdll,"JFree");
  jga=(JgaType)GETPROCADDRESS(hjdll,"Jga");
  jgeta=(JGetAType)GETPROCADDRESS(hjdll,"JGetA");
  jgetlocale=(JGetLocaleType)GETPROCADDRESS(hjdll,"JGetLocale");
  jseta=(JSetAType)GETPROCADDRESS(hjdll,"JSetA");
  return jt;
#endif

}

// ---------------------------------------------------------------------
// set path and pathdll (wpath also set for win)
// WIN arg is 0, Unix arg is argv[0]
void jepath(char* arg, char* lib)
{
  Q_UNUSED(arg);

#if defined(Q_OS_IOS) || defined(Q_OS_WASM) || defined(Q_OS_ANDROID)
  path[0]=0;
#elif defined(_WIN32)
  WCHAR wpath[PLEN];
  GetModuleFileNameW(0,wpath,_MAX_PATH);
  *(wcsrchr(wpath, '\\')) = 0;
  WideCharToMultiByte(CP_UTF8,0,wpath,1+(int)wcslen(wpath),path,PLEN,0,0);
#else
  struct stat st;
#define sz 4000
  char arg2[sz],arg3[sz];
  char* src,*snk;
  int n;
// fprintf(stderr,"arg0 %s\n",arg);
// try host dependent way to get path to executable
// use arg if they fail (arg command in PATH won't work)
#ifdef __MACH__
  uint32_t len=sz;
  n=_NSGetExecutablePath(arg2,&len);
  if (0!=n) strcat(arg2,arg);
#else
  n=readlink("/proc/self/exe",arg2,sizeof(arg2));
  if (-1==n) strcpy(arg2,arg);
  else arg2[n]=0;
#endif
// fprintf(stderr,"arg2 %s\n",arg2);
// arg2 is path (abs or relative) to executable or soft link
  n=readlink(arg2,arg3,sz);
  if (-1==n) strcpy(arg3,arg2);
  else arg3[n]=0;
// fprintf(stderr,"arg3 %s\n",arg3);
  strcpy(pathexec,arg3);
  if ('/'==*arg3)
    strcpy(path,arg3);
  else {
    char *c=getcwd(path,sizeof(path));
    Q_UNUSED(c);
    strcat(path,"/");
    strcat(path,arg3);
  }
  *(1+strrchr(path,'/'))=0;
// remove ./ and backoff ../
  snk=src=path;
  while (*src) {
    if ('/'==*src&&'.'==*(1+src)&&'.'==*(2+src)&&'/'==*(3+src)) {
      *snk=0;
      snk=strrchr(path,'/');
      snk=0==snk?path:snk;
      src+=3;
    } else if ('/'==*src&&'.'==*(1+src)&&'/'==*(2+src))
      src+=2;
    else
      *snk++=*src++;
  }
  *snk=0;
  snk=path+strlen(path)-1;
  if ('/'==*snk) *snk=0;
#endif

#if defined(Q_OS_ANDROID)
  strcpy(pathdll,JDLLNAME);
  strcat(pathdll,JDLLEXT);
#else
  strcpy(pathdll,path);
  strcat(pathdll,filesepx);
  strcat(pathdll,JDLLNAME);
  strcat(pathdll,JDLLEXT);
#endif

#if !defined(_WIN32) && !defined(Q_OS_IOS) && !defined(Q_OS_WASM) && !defined(Q_OS_ANDROID)
  if(stat(pathdll,&st)||strncmp(pathexec0,"/usr/bin/",9)||strncmp(pathexec0,"/usr/local/bin/",15)||strncmp(pathexec0,"/opt/homebrew/bin/",18)) {
    char pathpx[PLEN];
    if('/'==*pathexec) {
      if(!strncmp(pathexec,"/opt/homebrew/bin/",strlen("/opt/homebrew/bin/"))) {
        FHS=true;
        strcat(pathetcpx,"/opt/homebrew");
      } else if(!strncmp(pathexec,"/usr/local/bin/",strlen("/usr/local/bin/"))) {
        FHS=true;
        strcat(pathetcpx,"/usr/local");
      } else if(!strncmp(pathexec,"/usr/bin/",strlen("/usr/bin/"))) {
        FHS=true;
        pathetcpx[0]=0;
      }
    } else {
      strcpy(pathpx,"/opt/homebrew/bin/");
      strcat(pathpx,pathexec);
      if(!stat(pathpx,&st)) {
        FHS=true;
        strcat(pathetcpx,"/opt/homebrew");
      } else {
        strcpy(pathpx,"/usr/local/bin/");
        strcat(pathpx,pathexec);
        if(!stat(pathpx,&st)) {
          FHS=true;
          strcat(pathetcpx,"/usr/local");
        } else {
          strcpy(pathpx,"/usr/bin/");
          strcat(pathpx,pathexec);
          if(!stat(pathpx,&st)) {
            FHS=true;
            pathetcpx[0]=0;
          }
        }
      }
    }
    if (FHS) {
#if defined(__APPLE__)
      strcpy(pathdll,pathetcpx);
      strcat(pathdll,"/lib/");
      strcat(pathdll,JDLLNAME);
      strcat(pathdll,".");
      strcat(pathdll,JDLLVER);
      strcat(pathdll,JDLLEXT);
#else
      strcpy(pathdll,JDLLNAME);
      strcat(pathdll,JDLLEXT);
      strcat(pathdll,".");
      strcat(pathdll,JDLLVER);
#endif
    }
  }
#endif

  if (*lib) {
    if (filesep==*lib || ('\\'==filesep && ':'==lib[1]))
      strcpy(pathdll,lib); // absolute path
    else {
      strcpy(pathdll,path);
      strcat(pathdll,filesepx);
      strcat(pathdll,lib); // relative path
    }
  }
}

// ---------------------------------------------------------------------
void jefail(char* msg)
{
  strcpy(msg, "Load library ");
  strcat(msg, pathdll);
  strcat(msg," failed: ");
#ifdef _WIN32
  char buf[256];
  FormatMessageA(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  /* Default language */
    buf, (sizeof(buf)/sizeof(char)), 0);
  strcat(msg,buf);
#else
  char buf[256],ermsg[256];
  if(errno&&!strerror_r(errno,ermsg,256))strcat(buf,ermsg);
#endif
  strcat(msg,"\n");
}

// ---------------------------------------------------------------------
// build and run first sentence to set BINPATH, ARGV, and run profile
// arg is command line ready to set in ARGV_z_
// type is 0 normal, 1 -jprofile xxx, 2 ijx basic, 3 nothing
// profile[ARGV_z_=:...[BINPATH=:....
// profile is from BINPATH, ARGV, ijx basic, or nothing
int jefirst(int type,char* arg)
{
  int r;
#if !defined(Q_OS_IOS) && !defined(Q_OS_WASM) && !defined(Q_OS_ANDROID)
  char* p,*q;
#endif
  char* input=(char *)malloc(2000+strlen(arg));

#if defined(Q_OS_IOS) || defined(Q_OS_WASM) || defined(Q_OS_ANDROID)
  bool overwrite=true;
  documentsPath=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
//  documentsPath=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).section('/',0,-2);  // up 1 folder level
  homePath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  qDebug() << "documentsPath " << documentsPath ;
  qDebug() << "homePath " << homePath ;
  installPath=homePath + "/j";
  qDebug() << "installPath " << installPath;
  QFile f(installPath+"/build.txt");
  if(f.exists()) {
    if (f.open(QIODevice::ReadWrite | QFile::Text)) {
      QTextStream in(&f);
      int oldver = in.readLine().split(" ")[0].toInt();
      if (oldver>=QString(BUILD_VERSION).toInt()) overwrite=false;
      f.close();
    }
  }
  if(1 || overwrite) {
    if (f.open(QIODevice::ReadWrite | QFile::Text)) {
      QTextStream out(&f);
      out << BUILD_VERSION << Qt::endl;
      f.close();
    }
    QDir(installPath+"/bin").removeRecursively();
    QDir(installPath+"/system").removeRecursively();
    QDir(installPath+"/addons/ide/qt").removeRecursively();
#if defined(Q_OS_WASM) || defined(Q_OS_IOS)
    copyDirectoryNested(":/j/jlibrary",q2s(installPath).c_str());
    QDir(installPath+"/j/test").removeRecursively();
    copyDirectoryNested(":/jtest",q2s(installPath).c_str());
#elif defined(Q_OS_ANDROID)
    copyDirectoryNested("assets:/jlibrary",q2s(installPath).c_str());
    QDir::setCurrent(installPath);
    setenv("HOME",homePath.toUtf8().constData(),1);
    qDebug() << "j HOME path " << QString::fromUtf8(getenv("HOME"));
    if(!QFile(homePath+"/tmp").exists())
      mkdir((homePath+"/tmp").toUtf8().constData(), S_IRWXU | S_IRWXG | S_IRWXO);
    QFile::setPermissions(homePath+"/tmp",(QFile::Permission)0x7777);
    setenv("TMPDIR",(homePath+"/tmp").toUtf8().constData(),1);
    qDebug() << "TMPDIR: " << QString::fromUtf8(getenv("TMPDIR"));
#endif
    QDir::setCurrent(installPath);
    qDebug() << "application current path: " << QDir::currentPath();
  }
#endif

  if (QFile("assets:/welcome.ijs").exists()) {
    QFile("welcome.ijs").remove();
    QFile("assets:/welcome.ijs").copy("welcome.ijs");
    QFile::setPermissions("welcome.ijs",(QFile::Permission)0x6644);
  }

  *input=0;
  QFile sprofile(":/standalone/profile.txt");
  QFileInfo info=QFileInfo(sprofile);
  if (info.exists() && info.isFile() && info.size()>0) standAlone=true;
  if (standAlone) {
    bool done=false;
    qint64 ssize=info.size();
    if (sprofile.open(QFile::ReadOnly)) {
      char * sdata=(char *)malloc(ssize);
      QDataStream in(&sprofile);
      if (ssize==in.readRawData(sdata,ssize)) {
        jsetc((char *)"profile_jrx_",sdata, ssize);
        strcat(input,"0!:0 profile_jrx_[4!:55<'profile_jrx_'");
        done=true;
      }
      sprofile.close();
      free(sdata);
    }
    if (!done) {
      jsetc((char *)"profile_jrx_",(char *)"2!:55[1", 7);
      strcat(input,"0!:0 profile_jrx_[4!:55<'profile_jrx_'");
    }
  } else {
    if (0==type) {
      if (!FHS) {
#if defined(Q_OS_ANDROID)
        strcat(input,"(3 : '0!:0 y')<INSTALLROOT,'/bin");
#else
        strcat(input,"(3 : '0!:0 y')<BINPATH,'");
#endif
      } else {
        strcat(input,"(3 : '0!:0 y')<'");
        strcat(input,pathetcpx);
        strcat(input,"/etc/j/");
        strcat(input,JDLLVER);
      }
      strcat(input,filesepx);
      strcat(input,"profile.ijs'");
    } else if (1==type)
      strcat(input,"(3 : '0!:0 y')2{ARGV");
    else if (2==type)
      strcat(input,"");   // strcat(input,ijx);
    else
      strcat(input,"i.0 0");
  }
  strcat(input,"[ARGV_z_=:");
  strcat(input,arg);
#ifdef RASPI
  strcat(input,"[IFRASPI_z_=:1");
#endif
#if defined(Q_OS_WASM)
  strcat(input,"[UNAME_z_=:'Wasm'");
#elif defined(_WIN32)
  strcat(input,"[UNAME_z_=:'Win'");
#elif defined(__APPLE__)
  strcat(input,"[UNAME_z_=:'Darwin'");
#elif defined(__FreeBSD__)
  strcat(input,"[UNAME_z_=:'FreeBSD'");
#elif defined(__OpenBSD__)
  strcat(input,"[UNAME_z_=:'OpenBSD'");
#elif defined(Q_OS_ANDROID)
  strcat(input,"[UNAME_z_=:'Android'");
#else
  strcat(input,"[UNAME_z_=:'Linux'");
#endif
  strcat(input,"[BINPATH_z_=:'");
#if defined(Q_OS_IOS) || defined(Q_OS_WASM) || defined(Q_OS_ANDROID)
  strcat(input,q2s(installPath+"/bin").c_str());
  strcat(input,"'");
  strcat(input,"[INSTALLROOT_z_=:'");
  strcat(input,q2s(installPath).c_str());
#if defined(Q_OS_ANDROID)
  strcat(input,"'");
  strcat(input,"[AndroidPackage_z_=:'");
  strcat(input,q2s(AndroidPackage).c_str());
#endif
#else
  if(!FHS) {
    p=path;
    q=input+strlen(input);
    while (*p) {
      if (*p=='\'') *q++='\'';	// 's doubled
      *q++=*p++;
    }
    *q=0;
  } else {
    if(0==*pathetcpx) strcat(input,"/usr/bin");
    else {
      strcat(input,pathetcpx);
      strcat(input,"/bin");
    }
  }
#endif
  strcat(input,"'");

  strcat(input,"[LIBFILE_z_=:'");
#if !defined(Q_OS_IOS) && !defined(Q_OS_WASM) && !defined(Q_OS_ANDROID)
  p=pathdll;
  q=input+strlen(input);
  while (*p) {
    if (*p=='\'') *q++='\'';	// 's doubled
    *q++=*p++;
  }
  *q=0;
#endif
  strcat(input,"'");
  if (!FHS)
    strcat(input,"[FHS_z_=:0");
  else
    strcat(input,"[FHS_z_=:1");
  strcat(input,"[IFQT_z_=:1");
#if defined(Q_OS_IOS)
  strcat(input,"[IFIOS_z_=:1");
  strcat(input,"[IFIPAD_z_=:");
  strcat(input,isPad?"1":"0");
  strcat(input,"[IFRETINA_z_=:");
  strcat(input,isRetina?"1":"0");
#endif
  strcat(input,"[libjqt_z_=:'");
  strcat(input,q2s(LibName).c_str());
  strcat(input,"'");
  qDebug() << "j first line" << QString(input);
  r=jedo(input);
  if (r) {
    qDebug() << "j first line error: " << QString::number(r);
  }
  free(input);
  return r;
}

// ---------------------------------------------------------------------
void sigint(int k)
{
  Q_UNUSED(k);
  **adadbreak+=1;
  signal(SIGINT,sigint);
}

// ---------------------------------------------------------------------
// jdo with result (contains 3!:1 rep)
// return 0 on error
A dora(std::string s)
{
  if (sizeof(inputline)<8+s.size()) exit(100);
  strcpy(inputline,"r_jrx_=:");
  strcat(inputline,s.c_str());
  int e = jdo(jt,inputline);
  if (!e) {
    if (!jdo(jt,(C*)"q_jrx_=:4!:0<'r_jrx_'")) {
      A at=jgeta(jt,6,(char*)"q_jrx_");
      AREP p=(AREP)(sizeof(A_RECORD) + (char*)at);
      assert((0x1fffff&p->t)==4);
      assert((0xff&p->r)==0);
      return (0==*(I*)p->s)?jgeta(jt,6,(char*)"r_jrx_"):0;
    }
    return 0;
  } else
    return 0;
}

// ---------------------------------------------------------------------
// jdo with boolean result
bool dorb(std::string s)
{
  if (!jt) return false;
  A r=dora(s);
  if (!r) return false;
  AREP p=(AREP)(sizeof(A_RECORD) + (char*)r);
  assert((0x1fffff&p->t)==1);
  assert((0xff&p->r)==0);
  return !!*((char*)p->s);
}

// ---------------------------------------------------------------------
// jdo to get integer vector
// return success
bool doriv(std::string s,I**v,I*len)
{
  if (!jt) return false;
  A r=dora(s);
  if (!r) return false;
  AREP p=(AREP)(sizeof(A_RECORD) + (char*)r);
  assert((0x1fffff&p->t)==4);
  assert((0xff&p->r)<2);
  if ((0xff&p->r)==0) {
    *len = 1;
    *v=((I*)p->s);
  } else {
    *len = p->c;
    *v=(I*)(sizeof(AREP_RECORD)+(char*)p);
  }
  return true;
}

// ---------------------------------------------------------------------
// jdo with string result
std::string dors(std::string s)
{
  if (!jt) return "";
  A r=dora(s);
  if (!r) return "";
  AREP p=(AREP)(sizeof(A_RECORD) + (char*)r);
  if (!p->c) return "";
  assert((0x1fffff&p->t)==2);
  assert((0xff&p->r)<2);
  if ((0xff&p->r)==0)
    return std::string(((char*)p->s),1);
  else
    return std::string((sizeof(AREP_RECORD)+(char*)p), p->c);
}

// ---------------------------------------------------------------------
// set string in J
void sets(QString name, std::string s)
{
  int n,hlen,nlen,slen,tlen;

  I hdr[5];
  n=sizeof(I);
  hlen=n*5;

  QByteArray nb=name.toUtf8();
  nlen=nb.size();

  slen=(int)s.size();
  tlen=n*(1+slen/n);

//  hdr[0]=(4==n) ? 225 : 227;
  hdr[0]=0;
  unsigned char flag[1];
#if defined(BIGENDIAN)
  flag[0]=(4==n) ? 224 : 226;
#else
  flag[0]=(4==n) ? 225 : 227;
#endif
  memcpy(hdr,flag,1);
  hdr[1]=2;
  hdr[3]=1;
  hdr[2]=hdr[4]=slen;

  C* buf=(C*)calloc(hlen+tlen,sizeof(char));
  memcpy(buf,hdr,hlen);
  memcpy(buf+hlen,s.c_str(),slen);
  if (jt) jseta(jt,nlen,(C*)nb.constData(),(hlen+tlen),buf);
  free(buf);
}

// ---------------------------------------------------------------------
// set character in J
void jsetc(C* name, C* sb, I slen)
{
  int n,hlen,nlen,tlen;

  I hdr[5];
  n=sizeof(I);
  hlen=n*5;

  nlen=(int)strlen(name);

  tlen=n*(1+slen/n);

//  hdr[0]=(4==n) ? 225 : 227;
  hdr[0]=0;
  unsigned char flag[1];
#if defined(BIGENDIAN)
  flag[0]=(4==n) ? 224 : 226;
#else
  flag[0]=(4==n) ? 225 : 227;
#endif
  memcpy(hdr,flag,1);
  hdr[1]=2;
  hdr[3]=1;
  hdr[2]=hdr[4]=slen;

  C* buf=(C*)calloc(hlen+tlen,sizeof(char));
  memcpy(buf,hdr,hlen);
  memcpy(buf+hlen,sb,slen);
  if (jt) jseta(jt,nlen,name,(hlen+tlen),buf);
  free(buf);
}

// ---------------------------------------------------------------------
// get character in J
C* jgetc(C* name, I* len)
{
  A r = jgeta(jt,strlen(name),name);
  AREP p=(AREP)(sizeof(A_RECORD) + (char*)r);
  assert((0x1fffff&p->t)==2);
  assert((0xff&p->r)<2);
  if ((0xff&p->r)==0) {
    *len = 1;
    return (C*)((char*)p->s);
  } else {
    *len = p->c;
    return (C*)(sizeof(AREP_RECORD)+(char*)p);
  }
}

// ---------------------------------------------------------------------
void dumpA(A a)
{
  qDebug() << "k " << a->k;
  qDebug() << "flag " << a->flag;
  qDebug() << "m " << a->m;
  qDebug() << "t " << (0x1fffff&a->t);
  qDebug() << "c " << a->c;
  qDebug() << "n " << a->n;
  qDebug() << "r " << (0xff&a->r);
  qDebug() << "s " << a->s[0];
}

// ---------------------------------------------------------------------
void dumpAREP(AREP a)
{
  qDebug() << "n " << a->n;
  qDebug() << "t " << (0x1fffff&a->t);
  qDebug() << "c " << a->c;
  qDebug() << "r " << (0xff&a->r);
  qDebug() << "s " << a->s[0];
}
