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

#include "jlib.h"

#define BUFLEN 30000
#define PLEN 1000

int jedo(char* sentence);
void jefree();
void jefail(char* msg);
int jefirst(int type,char* arg);
JS jeload(void* callbacks);
void jepath(char* arg, char* lib);

void addargv(int argc, char* argv[], C* d);
void sigint(int k);

extern char inputline[BUFLEN+1];
extern char **adadbreak;
extern JS jt;
extern void * hjdll;

A dora(std::string s);
bool dorb(std::string s);
bool doriv(std::string s,I** v,I* len);
std::string dors(std::string s);
void dumpA(A a);
void dumpAREP(AREP a);
void sets(QString name, std::string s);
void jsetc(C * name, C* sb, I slen);
C* jgetc(C* name, I* len);
A jega(I t, I n, I r, I*s);
char* jegetlocale();

#endif
