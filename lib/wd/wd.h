#ifndef WD_H
#define WD_H

#include "../base/base.h"

// for wd only:
using namespace std;

extern "C" {
  Dllexport int wd(char *s,int slen,char *&r,int &len);
}

void error(string s);
bool invalidopt(string n,QStringList opt,string valid);
bool invalidoptn(string n,QStringList opt,string valid);
string remquotes(string s);
QString mb(string c,string p);
string sm(string c);
void wdactivateform();
int translateqkey(int);
void wdsetfocuspolicy(QWidget *widget,string p);
void wdsetsizepolicy(QWidget *widget,string p);
void wdsetwh(QWidget *widget,string p);
int wdstandardicon(string s);

#endif
