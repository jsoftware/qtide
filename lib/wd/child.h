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
  Child(std::string n, std::string s, Form *f, Pane *p);
  virtual ~Child();

  virtual std::string getsysdata();

  virtual void cmd(std::string p,std::string v);
  virtual std::string get(std::string p,std::string v);
  virtual void set(std::string p,std::string v);

  virtual std::string getfocuschain(bool);

  virtual void setform();
  virtual void setfocuspolicy(std::string p);
  virtual void setsizepolicy(std::string p);
  virtual std::string state()=0;

  virtual void setwh(std::string);
  virtual void setmaxwhv(std::string);
  virtual void setmaxwh(int,int);
  virtual void setminwhv(std::string);
  virtual void setminwh(int,int);

  bool grouped;
  std::string id;
  std::string eid;  // for event
  std::string event;
  std::string parms;
  std::string type;
  std::string locale;  // for isigraph
  std::string sysdata;
  std::string sysmodifiers;
  Form *pform;
  Pane *ppane;
  QWidget *ptab;
  QWidget *widget;

protected:
  void childStyle(QStringList);
};

#endif
