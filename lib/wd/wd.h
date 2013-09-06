#ifndef WD_H
#define WD_H

// for wd only:
using namespace std;

#include "../base/base.h"

extern "C" {
  Dllexport int wd(char *s,char *&r,int &len);
  Dllexport void wdactivateform();
  Dllexport void *wdgetparentid(void *s);
}

void error(string s);
string remquotes(string s);
QString mb(string p);

#endif
