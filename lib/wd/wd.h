#ifndef WD_H
#define WD_H

// for wd only:
// using namespace std;

#include "../base/base.h"

extern "C" {
  Dllexport int wd(char *s,int slen,char *&r,int &len);
}

void error(std::string s);
bool invalidopt(std::string n,QStringList opt,std::string valid);
bool invalidoptn(std::string n,QStringList opt,std::string valid);
bool notbothopt(std::string n,QStringList opt,std::string a,std::string b);
std::string remquotes(std::string s);
QString mb(std::string c,std::string p);
std::string sm(std::string c);
void wdactivateform();
int translateqkey(int);
void wdqQueueInit();
void wdqQueueAdd(bool addatfront);
void wdqQueueRemove();
void wdsetfocuspolicy(QWidget *widget,std::string p);
void wdsetsizepolicy(QWidget *widget,std::string p);
void wdsetwh(QWidget *widget,std::string p);
int wdstandardicon(std::string s);

#endif
