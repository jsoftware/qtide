#ifndef WD_H
#define WD_H

// for wd only:
using namespace std;

#include "../base/base.h"

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
void wdqQueueInit();
void wdqQueueAdd(bool addatfront);
void wdqQueueRemove();
void wdsetfocuspolicy(QWidget *widget,string p);
void wdsetsizepolicy(QWidget *widget,string p);
void wdsetwh(QWidget *widget,string p);
int wdstandardicon(string s);

#endif
