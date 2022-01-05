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
  Form(std::string id, std::string p, std::string locale, QWidget *parent = 0);
  ~Form();
  QWidget *activetab();
  void addchild(Child *);
  Pane *addpane(int n);
  void addmenu();
  void closepane();
  void closeit();

  bool activeonly;
  std::string hschild();
  std::string hsform();
  std::string qform();
  bool nochild();
  std::string get(std::string p,std::string v);
  void saveformpos();
  void set(std::string p,std::string v);
  void setstretch(Child *cc, std::string factor);
  Child *setmenuid(std::string id);
  void setpadding(int l,int t,int r,int b);
  void setpn(std::string p);
  void setpicon(std::string p);
  void settaborder(std::string p);
  void settheme(std::string p);
  void settimer(std::string p);
  void showit(std::string p);
  void signalevent(Child *c, QKeyEvent *e = 0);
  std::string state(int evt);
  void status(std::string);
  std::string getsysmodifiers();
  std::string getsysmodifiers(Qt::KeyboardModifiers);
  std::string getfocus();

  bool closed;
  int maxvisibleitems;
  bool savepos;
  bool shown;
  int seq;
  std::string id;
  std::string event;
  std::string lasttype;
  std::string locale;
  std::string sysdata;
  std::string sysmodifiers;

  Child *child;
  Child *evtchild;
  QList<Child *>children;
  Menus *menubar;
  Pane *pane;
  QList<Pane *>panes;
  QWidget *tabpane;
  QTimer *timer;
  Tabs *tab;
  QList<Tabs *>tabs;
  QSignalMapper *signalMapper;
  QBoxLayout *layout;

public slots:
  void buttonClicked(QWidget *);
  Child *id2child(std::string n);
  bool ischild(Child* n);
  void systimer();

protected:
  void changeEvent(QEvent *);
  void closeEvent(QCloseEvent *);
  void keyPressEvent(QKeyEvent *e);
  void resizeEvent(QResizeEvent *event);

private slots:

private:
  bool closeok;
  bool escclose;
  std::string fakeid;
  std::string lastfocus;

};

extern Form *evtform;
extern int FormSeq;
extern QList<Form *>Forms;

#endif
