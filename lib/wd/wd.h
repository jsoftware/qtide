#ifndef WD_H
#define WD_H

#include "../base/base.h"

// for wd only:
using namespace std;

extern "C" {
  Dllexport int wd(char *s,int slen,char *&r,int &len);
}

void error(string s);
string remquotes(string s);
QString mb(string c,string p);
string sm(string c,string p);
void wdactivateform();
int translateqkey(int);
void wdsetfocuspolicy(QWidget *widget,string p);
void wdsetsizepolicy(QWidget *widget,string p);
void wdsetwh(QWidget *widget,string p);

extern QStringList defChildStyle;

#endif
