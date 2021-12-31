#ifndef PANE_H
#define PANE_H

#include <QWidget>
#include <QCloseEvent>

class QGridLayout;
class QBoxLayout;
class QButtonGroup;
class QGroupBox;
class QSplitter;

class Child;
class Form;
class GroupBox;
class Layout;

#include "wd.h"
#include "form.h"
#include "font.h"
#include "child.h"
#include "layout.h"

class Pane : public QWidget
{
  Q_OBJECT

public:
  Pane(int n,Form *f);
  bool addchild(string n,string c,string p);
  void addlayout(Layout *b);
  void bin(string c);
  void fini();
  void grid(string c, string s);
  bool groupbox(string c, string s);
  bool line(string c, string s);
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
  int maxsizew;
  int maxsizeh;
  int minsizew;
  int minsizeh;

  QButtonGroup *buttongroup;
  Child *child;
  Child *evtchild;
  QGroupBox *groupboxw;
  Layout *layout;
  QList<Layout *>layouts;
  QSplitter *qsplitter;
  QList<int> qsplitterp;

};

#endif
