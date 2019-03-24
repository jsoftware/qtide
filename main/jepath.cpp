/* Copyright 1990-2019, Jsoftware Inc.  All rights reserved. */
#define PLEN 1000 // path length
#ifdef _WIN32
#include <windows.h>

#define GETPROCADDRESS(h,p) GetProcAddress(h,p)
#define filesep '\\'
#define filesepx "\\"

#else
#ifdef __MACH__
#include <mach-o/dyld.h>        /* _NSGetExecutablePath */
#endif
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <dlfcn.h>
#define GETPROCADDRESS(h,p)	dlsym(h,p)
#define _stdcall
#define filesep '/'
#define filesepx "/"
#endif

static char path[PLEN];

// set path and pathdll (wpath also set for win)
// WIN arg is 0, Unix arg is argv[0]
extern "C" char * jepath1(char* arg)
{
#ifdef _WIN32
  arg=arg;
  WCHAR wpath[PLEN];
  GetModuleFileNameW(0,wpath,_MAX_PATH);
  *(wcsrchr(wpath, '\\')) = 0;
  WideCharToMultiByte(CP_UTF8,0,wpath,1+(int)wcslen(wpath),path,PLEN,0,0);
#else

#define sz 4000
  char arg2[sz],arg3[sz];
  char* src,*snk;
  ssize_t n;
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
    snk=getcwd(path,sizeof(path));
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
  return path;
}
