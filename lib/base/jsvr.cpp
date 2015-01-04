#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>

#include <csignal>
#include <assert.h>

#ifdef __MACH__
#include <mach-o/dyld.h>
#endif

#include "jsvr.h"
#include "util.h"

#ifdef QT_OS_ANDROID
#include <QDir>
#include <sys/stat.h>
extern QString AndroidPackage;
#endif
void * jdllproc=0;
void * jdlljt=0;

using namespace std;

typedef void* (_stdcall *JInitType)     ();
typedef int   (_stdcall *JDoType)       (void*, C*);
typedef A     (_stdcall *JGetAType)     (void*, I n, C*);
typedef C*    (_stdcall *JGetLocaleType)(void*);
typedef void  (_stdcall *JSMType)       (void*, void*);
typedef void  (_stdcall *JFreeType)     (void*);
typedef A     (_stdcall *JgaType)       (J jt, I t, I n, I r, I*s);
typedef int   (_stdcall *JSetAType)     (void*, I n, C*, I x, C*);

typedef void  (_stdcall * outputtype)(J,int,C*);
typedef int   (_stdcall * dowdtype)  (J,int, A, A*);
typedef C* (_stdcall * inputtype) (J,C*);

int _stdcall JDo(J jt,C*);                   /* run sentence */
C* _stdcall JGetLocale(J jt);                /* get locale */
J _stdcall JInit();                          /* init instance */
int _stdcall JFree(J jt);                    /* free instance */
A _stdcall JGetA(J jt,I n,C* name);          /* get 3!:1 from name */
I _stdcall JSetA(J jt,I n,C* name,I x,C* d); /* name=:3!:2 data */
void _stdcall JSM(J jt, void*callbacks[]);  /* set callbacks */

A jega(I t, I n, I r, I*s);
char* jegetlocale();
void* jehjdll();

char **adadbreak;
char inputline[BUFLEN+1];
J jt=0;

static char path[PLEN];
static char pathdll[PLEN];

static JDoType jdo;
static JFreeType jfree;
static JgaType jga;
static JGetAType jgeta;
static JGetLocaleType jgetlocale;
static void* hjdll;
static JSetAType jseta;

extern QString LibName;
bool FHS;
bool standAlone=false;

// ---------------------------------------------------------------------
void addargv(int argc, char* argv[], C* d)
{
  C *p,*q;
  I i;

  p=d+strlen(d);
  for(i=0; i<argc; ++i) {
    if(sizeof(inputline)<(100+strlen(d)+2*strlen(argv[i]))) exit(100);
    if(1==argc) {
      *p++=',';
      *p++='<';
    }
    if(i)*p++=';';
    *p++='\'';
    q=argv[i];
    while(*q) {
      *p++=*q++;
      if('\''==*(p-1))*p++='\'';
    }
    *p++='\'';
  }
  *p=0;
}

// ---------------------------------------------------------------------
int jedo(char* sentence)
{
  if (!jt) return 0;
  return jdo(jt,sentence);
}

// ---------------------------------------------------------------------
void jefree()
{
  if (jt) jfree(jt);
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
J jeload(void* callbacks)
{
  if (!jdllproc && (void*)-1!=jdlljt) return 0;
#ifdef _WIN32
  WCHAR wpath[PLEN];
  if (jdllproc) {
    hjdll=jdllproc;
  } else {
    MultiByteToWideChar(CP_UTF8,0,pathdll,1+(int)strlen(pathdll),wpath,PLEN);
    hjdll=LoadLibraryW(wpath);
  }
  if(!hjdll)return 0;
  jt=(jdllproc)?jdlljt:((JInitType)GETPROCADDRESS((HMODULE)hjdll,"JInit"))();
  if(!jt) return 0;
  if (!jdllproc) ((JSMType)GETPROCADDRESS((HMODULE)hjdll,"JSM"))(jt,callbacks);
  jdo=(JDoType)GETPROCADDRESS((HMODULE)hjdll,"JDo");
  jfree=(JFreeType)GETPROCADDRESS((HMODULE)hjdll,"JFree");
  jga=(JgaType)GETPROCADDRESS((HMODULE)hjdll,"Jga");
  jgeta=(JGetAType)GETPROCADDRESS((HMODULE)hjdll,"JGetA");
  jgetlocale=(JGetLocaleType)GETPROCADDRESS((HMODULE)hjdll,"JGetLocale");
  jseta=(JSetAType)GETPROCADDRESS((HMODULE)hjdll,"JSetA");
  return jt;
#else
  hjdll=(jdllproc)?jdllproc:dlopen(pathdll,RTLD_LAZY);
  if(!hjdll)return 0;
  jt=(jdllproc)?jdlljt:((JInitType)GETPROCADDRESS(hjdll,"JInit"))();
  if(!jt) return 0;
  if (!jdllproc) ((JSMType)GETPROCADDRESS(hjdll,"JSM"))(jt,callbacks);
  jdo=(JDoType)GETPROCADDRESS(hjdll,"JDo");
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
void jepath(char* arg)
{
  Q_UNUSED(arg);
#if !defined(QT_OS_ANDROID)
  if (FHS) {
    strcpy(pathdll,JDLLNAME);
#if defined(_WIN32_)
    *(strrchr(pathdll,'.')) = 0;
    strcat(pathdll,"-" JDLLVER);
    strcat(pathdll,".dll");
#else
    strcat(pathdll,"." JDLLVER);
#endif
    return;
  }
#endif
#ifdef _WIN32
  WCHAR wpath[PLEN];
  GetModuleFileNameW(0,wpath,_MAX_PATH);
  *(wcsrchr(wpath, '\\')) = 0;
  WideCharToMultiByte(CP_UTF8,0,wpath,1+(int)wcslen(wpath),path,PLEN,0,0);
#elif defined(QT_OS_ANDROID)
  QFileInfo fileInfo(LibName);
  strcpy(path,fileInfo.canonicalPath().toUtf8().constData());
  qDebug() << "jepath " << s2q(path);
#else
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
  if(0!=n) strcat(arg2,arg);
#else
  n=readlink("/proc/self/exe",arg2,sizeof(arg2));
  if(-1==n) strcpy(arg2,arg);
  else arg2[n]=0;
#endif
// fprintf(stderr,"arg2 %s\n",arg2);
// arg2 is path (abs or relative) to executable or soft link
  n=readlink(arg2,arg3,sz);
  if(-1==n) strcpy(arg3,arg2);
  else arg3[n]=0;
// fprintf(stderr,"arg3 %s\n",arg3);
  if('/'==*arg3)
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
  while(*src) {
    if('/'==*src&&'.'==*(1+src)&&'.'==*(2+src)&&'/'==*(3+src)) {
      *snk=0;
      snk=strrchr(path,'/');
      snk=0==snk?path:snk;
      src+=3;
    } else if('/'==*src&&'.'==*(1+src)&&'/'==*(2+src))
      src+=2;
    else
      *snk++=*src++;
  }
  *snk=0;
  snk=path+strlen(path)-1;
  if('/'==*snk) *snk=0;
#endif
  strcpy(pathdll,path);
  strcat(pathdll,filesepx );
  strcat(pathdll,JDLLNAME);
// fprintf(stderr,"arg4 %s\n",path);
}

// ---------------------------------------------------------------------
void jefail(char* msg)
{
  strcpy(msg, "Load library ");
  strcat(msg, pathdll);
  strcat(msg," failed.");
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
  char* p,*q;
  char* input=(char *)malloc(2000+strlen(arg));

#ifdef QT_OS_ANDROID
  Q_UNUSED(p);
  Q_UNUSED(q);
  char *homepath;
  char *sdcard;
  char *SDCARD[] = {
    (char *)"/storage/emulated/0",
    (char *)"/storage/emulated/legacy",
    (char *)"/mnt/sdcard"
  };
  struct stat st;
  if (!(sdcard=getenv("EXTERNAL_STORAGE"))) {
    for (int i=0; i < 3; i++) {
      if (!stat(SDCARD[i],&st)) {
        sdcard = SDCARD[i];
        break;
      }
    }
  }
  int sdcardok = sdcard && !stat(sdcard,&st);
//  sdcardok = 0;
  if (sdcardok) setenv("EXTERNAL_STORAGE",sdcard,1);

// a dummy file signifying internal install for scripts
  if (QFile("assets:/internal_install.txt").exists()) sdcardok = 0;
  if(sdcardok)
    setenv("HOME",sdcard,1);
  else {
    if(!getenv("HOME"))
      setenv("HOME",QDir::currentPath().toUtf8().constData(),1);
  }
  homepath=getenv("HOME");
  qDebug() << "homepath: " << s2q(homepath);
  QString appcurrentpath = QDir::currentPath();
  qDebug() << "application current path: " << appcurrentpath;
// if(!getenv("TMPDIR"))
//   setenv("TMPDIR",QDir::tempPath().toUtf8().constData(),1);
  if(!QFile(appcurrentpath+"/tmp").exists()) mkdir((appcurrentpath+"/tmp").toUtf8().constData(), S_IRWXU | S_IRWXG | S_IRWXO);
  QFile::setPermissions(appcurrentpath+"/tmp",(QFile::Permission)0x7777);
  setenv("TMPDIR",(appcurrentpath+"/tmp").toUtf8().constData(),1);

  qDebug() << "TMPDIR: " << QString::fromUtf8(getenv("TMPDIR"));

  char install[PLEN];
  if (sdcardok) {
    strcpy(install, sdcard);
    strcat(install, "/Android/data");
    strcat(install, "/");
    strcat(install, AndroidPackage.toUtf8().constData());
    if(stat(install,&st)) mkdir(install, S_IRWXU | S_IRWXG | S_IRWXO);
    strcat(install, "/files");
    if(stat(install,&st)) mkdir(install, S_IRWXU | S_IRWXG | S_IRWXO);
    if(stat(install,&st)) {
      qDebug() << "can not mkdir install: " << s2q(install);
      strcpy(install, appcurrentpath.toUtf8().constData());
    }
  } else {
    strcpy(install, appcurrentpath.toUtf8().constData());
  }
  qDebug() << "install path: " << s2q(install);

  QDir::setCurrent(install);
  qDebug() << "current path: " << QDir::currentPath();
// assume cwd is .../files

  QString v1, v2;
  QFile *f1 = new QFile("assets:/assets_version.txt");
  QFile *f2 = new QFile("assets_version.txt");
  if (f1->exists()) {
    v1= cfread(f1);
  }
  if (f2->exists()) {
    v2= cfread(f2);
  }
  delete f1;
  delete f2;
  if (v1!=v2 && QFile("assets:/jqtdata.tgz").exists() && QFile("assets:/tar0.ijs").exists()) {
    qDebug() << "decompress assets";
    QFile("jqtdata.tgz").remove();
    QFile("tar0.ijs").remove();
    QFile("assets:/jqtdata.tgz").copy("jqtdata.tgz");
    QFile("assets:/tar0.ijs").copy("tar0.ijs");
    QFile::setPermissions("jqtdata.tgz",(QFile::Permission)0x6666);  // for busybox tar
    QFile::setPermissions("tar0.ijs",(QFile::Permission)0x6644);
// for testing, display script
//    int e=jedo((char *)"script0=: [: 3 : '0!:1 y [ 4!:55<''y''' ]&.:>");
    int e=jedo((char *)"script0=: [: 3 : '0!:0 y [ 4!:55<''y''' ]&.:>");
    if (!e) {
      e=jedo((char *)"(i.0 0)[4!:55 ::0:<'script0'[18!:55 ::0:<'j'[18!:55 ::0:<'jtar0'[tar_jtar0_ ::0:'x';'jqtdata.tgz';'.'[script0 ::0:'tar0.ijs'");
      if (e) {
        qDebug() << "script0 error:" << QString::number(e);
      }
    } else {
      qDebug() << "can not set script0";
    }
    QFile("jqtdata.tgz").remove();
    QFile("tar0.ijs").remove();
    QFile("assets_version.txt").remove();
    QFile("assets:/assets_version.txt").copy("assets_version.txt");
    QFile::setPermissions("assets_version.txt",(QFile::Permission)0x6644);

    if (QFile("assets:/jconsole").exists()) {
      if (!QFile(appcurrentpath+"/bin").exists()) mkdir((appcurrentpath+"/bin").toUtf8().constData(), S_IRWXU | S_IRWXG | S_IRWXO);
      QFile::setPermissions(appcurrentpath+"/bin",(QFile::Permission)0x7755);
      QFile(appcurrentpath+"/bin/jconsole").remove();
      QFile("assets:/jconsole").copy(appcurrentpath+"/bin/jconsole");
      QFile::setPermissions(appcurrentpath+"/bin/jconsole",(QFile::Permission)0x7755);
      qDebug() << "jconsole: " << (appcurrentpath+"/bin/jconsole");
    }
  }

  if (QFile("assets:/welcome.ijs").exists()) {
    QFile("welcome.ijs").remove();
    QFile("assets:/welcome.ijs").copy("welcome.ijs");
    QFile::setPermissions("welcome.ijs",(QFile::Permission)0x6644);
  }

  QDir::setCurrent(homepath);
  qDebug() << "current path :(home) " << QDir::currentPath();
#endif

  *input=0;
  QFile sprofile(":/standalone/profile.ijs");
  QFileInfo info=QFileInfo(sprofile);
  if (info.exists() && info.isFile() && info.size()>0) standAlone=true;
  if (standAlone) {
    bool done=false;
    qint64 ssize=info.size();
    if(sprofile.open(QFile::ReadOnly)) {
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
    if(0==type) {
#if defined(QT_OS_ANDROID)
      strcat(input,"(3 : '0!:0 y')<INSTALLROOT,'/bin");
#else
      if (!FHS) {
        strcat(input,"(3 : '0!:0 y')<BINPATH,'");
      } else {
#if defined(_WIN32)
        strcat(input,"(3 : '0!:0 y')<(2!:5'ALLUSERSPROFILE')'\\j\\" JDLLVER);
#else
        strcat(input,"(3 : '0!:0 y')<'/etc/j/" JDLLVER);
#endif
      }
#endif
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
  strcat(input,"[BINPATH_z_=:'");
#ifdef QT_OS_ANDROID
  strcat(input,appcurrentpath.toUtf8().constData());
  strcat(input,"/bin'");
  strcat(input,"[UNAME_z_=:'Android'");
  strcat(input,"[INSTALLROOT_z_=:'");
  strcat(input,install);
  strcat(input,"'");
  strcat(input,"[AndroidPackage_z_=:'");
  strcat(input,AndroidPackage.toUtf8().constData());
  strcat(input,"'");
#else
  p=path;
  q=input+strlen(input);
  while(*p) {
    if(*p=='\'') *q++='\'';	// 's doubled
    *q++=*p++;
  }
  *q=0;
  strcat(input,"'");
#endif
  if (!FHS)
    strcat(input,"[FHS_z_=:0");
  else
    strcat(input,"[FHS_z_=:1");
  strcat(input,"[IFQT_z_=:1");
  strcat(input,"[libjqt_z_=:'");
  strcat(input,LibName.toUtf8().constData());
  strcat(input,"'");
#ifdef QT_OS_ANDROID
  qDebug() << "j first line: " << QString::fromUtf8(input);
#endif
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
A dora(string s)
{
  if (sizeof(inputline)<8+s.size()) exit(100);
  strcpy(inputline,"r_jrx_=:");
  strcat(inputline,s.c_str());
  int e = jdo(jt,inputline);
  if (!e) {
    if (!jdo(jt,(C*)"q_jrx_=:4!:0<'r_jrx_'")) {
      A at=jgeta(jt,6,(char*)"q_jrx_");
      AREP p=(AREP) (sizeof(A_RECORD) + (char*)at);
      assert(p->t==4);
      assert(p->r==0);
      return (0==*(I*)p->s)?jgeta(jt,6,(char*)"r_jrx_"):0;
    }
    return 0;
  } else
    return 0;
}

// ---------------------------------------------------------------------
// jdo to get integer vector
// return success
bool doriv(string s,I**v,I*len)
{
  string t;
  if (!jt) return false;
  A r=dora(s);
  if (!r) return false;
  AREP p=(AREP) (sizeof(A_RECORD) + (char*)r);
  assert(p->t==4);
  assert(p->r<2);
  if (p->r==0) {
    *len = 1;
    return ((I*)p->s);
  } else {
    *len = p->c;
    *v=(I*)(sizeof(AREP_RECORD)+(char*)p);
  }
  return true;
}

// ---------------------------------------------------------------------
// jdo with QString result
string dors(string s)
{
  string t;
  if (!jt) return "";
  A r=dora(s);
  if (!r) return "";
  AREP p=(AREP) (sizeof(A_RECORD) + (char*)r);
  assert(p->t==2);
  assert(p->r<2);
  if (p->r==0)
    return string(((char*)p->s),1);
  else
    return string((sizeof(AREP_RECORD)+(char*)p), p->c);
}

// ---------------------------------------------------------------------
// set string in J
void sets(QString name, string s)
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
  AREP p=(AREP) (sizeof(A_RECORD) + (char*)r);
  assert(p->t==2);
  assert(p->r<2);
  if (p->r==0) {
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
  qDebug() << "t " << a->t;
  qDebug() << "c " << a->c;
  qDebug() << "n " << a->n;
  qDebug() << "r " << a->r;
  qDebug() << "s " << a->s[0];
}

// ---------------------------------------------------------------------
void dumpAREP(AREP a)
{
  qDebug() << "n " << a->n;
  qDebug() << "t " << a->t;
  qDebug() << "c " << a->c;
  qDebug() << "r " << a->r;
  qDebug() << "s " << a->s[0];
}
