#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QCloseEvent>

class QButtonGroup;
class QMenuBar;
class QSignalMapper;
class QBoxLayout;

class Child;
class Menus;

#include "wd.h"
#include "form.h"
#include "font.h"
#include "child.h"

class Form : public QWidget
{
  Q_OBJECT

public:
  Form(string id, string p, string locale, QWidget *parent = 0);
  ~Form();
  bool addchild(string n,string c,string p);
  void addlayout(QBoxLayout *b, int n);
  void addmenu();
  void bin(string c);
  string hschild();
  string hsform();
  bool nochild();
  void setstretch(Child *cc, string factor);
  Child *setmenuid(string id);
  void setpn(string p);
  void showit();
  void signalevent(Child *c);
  string state(int evt);

  bool closed;
  bool shown;

  string id;
  string event;
  string lasttype;
  string locale;
  string sysdata;
  string sysmodifiers;
  int sizew;
  int sizeh;

  QButtonGroup *buttongroup;
  Font *fontdef;
  Child *child;
  Child *evtchild;
  QList<Child *>children;
  Menus *menubar;
  QSignalMapper *signalMapper;
  QBoxLayout *layout;
  QList<QBoxLayout *>layouts;
  QList<int>layoutx;

public slots:
  void buttonClicked(QWidget *);
  Child *id2child(string n);
  bool ischild(Child* n);

protected:
  void closeEvent(QCloseEvent *);
  void keyPressEvent(QKeyEvent *e);

private:
  bool closeok;
  bool escclose;

};

extern Form *form;
extern Form *evtform;
extern QList<Form *>Forms;

#endif
