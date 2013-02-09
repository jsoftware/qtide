#ifndef PANE_H
#define PANE_H

#include <QWidget>
#include <QCloseEvent>

class QBoxLayout;
class QButtonGroup;
class QSignalMapper;
class QSplitter;

class Child;
class Form;

#include "wd.h"
#include "form.h"
#include "font.h"
#include "child.h"

class Pane : public QWidget
{
  Q_OBJECT

public:
  Pane(int n,Form *f);
  bool addchild(string n,string c,string p);
  void addlayout(QBoxLayout *b, int n);
  void bin(string c);
  void fini();
  string hschild();
  string hsform();
  bool nochild();
  void setstretch(Child *cc, string factor);
  Child *setmenuid(string id);
  void setpn(string p);
  void showit();
  void signalevent(Child *c);
  bool split(string c, string s);
  void splitend();
  string state(int evt);

  Form *pform;
  string event;
  string lasttype;
  string locale;
  string sysdata;
  string sysmodifiers;
  int sizew;
  int sizeh;

  QButtonGroup *buttongroup;
  Child *child;
  Child *evtchild;
  QSignalMapper *signalMapper;
  QBoxLayout *layout;
  QList<QBoxLayout *>layouts;
  QList<int>layoutx;
  QSplitter *qsplit;
  QList<int> qsplitp;

};

#endif
