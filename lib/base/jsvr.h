#ifndef JFE_H
#define JFE_H

// define _WIN32 for Windows, __MACH__ for MAC, J64 for 64-bit
// JE is loaded from current working directory

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define GETPROCADDRESS(h,p) GetProcAddress(h,p)
#define JDLLNAME "j"
#define JDLLEXT ".dll"
#define filesep '\\'
#define filesepx "\\"
#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif
#else
#define _stdcall
#include <unistd.h>
#include <dlfcn.h>
#define GETPROCADDRESS(h,p)	dlsym(h,p)
#define JDLLNAME "libj"
#ifdef __MACH__
#define JDLLEXT ".dylib"
#else
#include <sys/utsname.h>
#define JDLLEXT ".so"
#endif
#define _getcwd getcwd
#define filesep '/'
#define filesepx "/"
#endif

#define BUFLEN 30000
#define PLEN 1000

// smoptions
#define SMWIN    0  /* j.exe    Jwdw (Windows) front end */
#define SMJAVA   2  /* j.jar    Jwdp (Java) front end */
#define SMCON    3  /* jconsole */
#define SMQT     4  /* jqt */

#if defined(_WIN64)||defined(__LP64__)
typedef long long I;
#define FMTI            "%lli"
#else
typedef long I;
#define FMTI            "%li"
#endif

typedef char C;
typedef void* J;

typedef struct A_RECORD {
  I k,flag,m,t,c,n,r,s[1];
}* A;

typedef struct AREP_RECORD {
  I n,t,c,r,s[1];
}* AREP;

int jedo(char* sentence);
void jefree();
void jefail(char* msg);
int jefirst(int type,char* arg);
J jeload(void* callbacks);
void jepath(char* arg, char* lib);

void addargv(int argc, char* argv[], C* d);
void sigint(int k);

extern char inputline[BUFLEN+1];
extern char **adadbreak;
extern J jt;
extern void * hjdll;
extern void * jdllproc;
extern void * jdlljt;

A dora(std::string s);
bool dorb(std::string s);
bool doriv(std::string s,I** v,I* len);
std::string dors(std::string s);
void dumpA(A a);
void dumpAREP(AREP a);
void sets(QString name, std::string s);
void jsetc(C * name, C* sb, I slen);
C* jgetc(C* name, I* len);

#endif
