#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QEvent>

class QMenuBar;
class QSignalMapper;
class QBoxLayout;
class QTimer;

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
  string qform();
  bool nochild();
  string get(string p,string v);
  void set(string p,string v);
  void setstretch(Child *cc, string factor);
  Child *setmenuid(string id);
  void setpadding(int l,int t,int r,int b);
  void setpn(string p);
  void setpicon(string p);
  void settaborder(string p);
  void settimer(string p);
  void showit(string p);
  void signalevent(Child *c, QKeyEvent *e = 0);
  string state(int evt);
  void status(string);
  string getsysmodifiers();
  string getfocus();

  bool closed;
  bool shown;
  int seq;
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
  QTimer *timer;
  Tabs *tab;
  QList<Tabs *>tabs;
  QSignalMapper *signalMapper;
  QBoxLayout *layout;

public slots:
  void buttonClicked(QWidget *);
  Child *id2child(string n);
  bool ischild(Child* n);
  void systimer();

protected:
  void changeEvent(QEvent *);
  void closeEvent(QCloseEvent *);
  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);

private slots:
#ifdef QT_OS_ANDROID
  void backButtonTimer();
#endif

private:
#ifdef QT_OS_ANDROID
  bool backButtonPressed;
#endif
  bool closeok;
  bool escclose;
  string fakeid;
  string lastfocus;

};

extern Child *isigraph;
extern Child *opengl;
extern Form *evtform;
extern int FormSeq;
extern QList<Form *>Forms;

#endif
