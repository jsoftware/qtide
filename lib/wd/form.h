#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QCloseEvent>

class QMenuBar;
class QSignalMapper;
class QBoxLayout;

class Child;
class Menus;
class Pane;
class Tabs;

#include "wd.h"
#include "form.h"
#include "pane.h"
#include "font.h"
#include "child.h"

class Form : public QWidget
{
  Q_OBJECT

public:
  Form(string id, string p, string locale, QWidget *parent = 0);
  ~Form();
  void addchild(Child *);
  Pane *addpane(int n);
  void addmenu();
  void closepane();

  string hschild();
  string hsform();
  string qformx();
  bool nochild();
  void setstretch(Child *cc, string factor);
  Child *setmenuid(string id);
  void setpadding(int l,int t,int r,int b);
  void setpn(string p);
  void showit();
  void signalevent(Child *c);
  string state(int evt);
  void status(string);

  bool closed;
  bool shown;

  string id;
  string event;
  string lasttype;
  string locale;
  string sysdata;
  string sysmodifiers;

  Child *child;
  Child *evtchild;
  QList<Child *>children;
  Menus *menubar;
  Pane *pane;
  QList<Pane *>panes;
  Tabs *tab;
  QList<Tabs *>tabs;
  QSignalMapper *signalMapper;
  QBoxLayout *layout;

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
