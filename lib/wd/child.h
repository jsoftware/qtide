#ifndef CHILD_H
#define CHILD_H

#include <string>
#include <QWidget>
#include <QCloseEvent>

#include "../base/base.h"
#include "wd.h"

class Form;
class Pane;

class Child : public QObject
{
  Q_OBJECT

public:
  Child(string n, string s, Form *f, Pane *p);
  virtual ~Child();

  virtual string getsysdata();

  virtual void cmd(string p,string v);
  virtual string get(string p,string v);
  virtual void set(string p,string v);

  virtual string getfocuschain(bool);

  virtual void setform();
  virtual void setfocuspolicy(string p);
  virtual void setsizepolicy(string p);
  virtual string state()=0;

  virtual void setwh(string);
  virtual void setmaxwhv(string);
  virtual void setmaxwh(int,int);
  virtual void setminwhv(string);
  virtual void setminwh(int,int);

  bool grouped;
  string id;
  string eid;  // for event
  string event;
  string parms;
  string type;
  string locale;  // for isigraph
  string sysdata;
  string sysmodifiers;
  Form *pform;
  Pane *ppane;
  QWidget *ptab;
  QWidget *widget;

protected:
  void childStyle(QStringList);
};

#endif
