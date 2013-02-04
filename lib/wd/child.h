#ifndef CHILD_H
#define CHILD_H

#include <string>
#include <QWidget>
#include <QCloseEvent>

#include "wd.h"

class QVBoxLayout;
class Form;

class Child : public QObject
{
  Q_OBJECT

public:
  Child(string n, string p, Form *f);

  virtual string getsysmodifiers();
  virtual string getsysdata();
  virtual void set(string p);
  virtual void setenable(string p);
  virtual void setfont(QFont);
  virtual void setp(string p,string v);
  virtual void setform();
  virtual string state();

  virtual void setxywh(string,int);
  virtual void setminwh(int,int);

  bool grouped;
  string id;
  string event;
  string parms;
  string type;
  string locale;  // for isigraph
  string sysdata;
  string sysmodifiers;
  Form *pform;
  QWidget *widget;

};

#endif


