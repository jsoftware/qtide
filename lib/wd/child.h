#ifndef CHILD_H
#define CHILD_H

#include <string>
#include <QWidget>
#include <QCloseEvent>

#include "wd.h"

class Form;
class Pane;

class Child : public QObject
{
  Q_OBJECT

public:
  Child(string n, string s, Form *f, Pane *p);

  virtual string getsysmodifiers();
  virtual string getsysdata();
  virtual void set(string p);
  virtual void setenable(string p);
  virtual void setfont(QFont);
  virtual void setp(string p,string v);
  virtual void setform();
  virtual void setstylesheet(string p);
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
  Pane *ppane;
  QWidget *widget;

};

#endif
