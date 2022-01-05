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
  bool addchild(std::string n,std::string c,std::string p);
  void addlayout(Layout *b);
  void bin(std::string c);
  void fini();
  void grid(std::string c, std::string s);
  bool groupbox(std::string c, std::string s);
  bool line(std::string c, std::string s);
  std::string hschild();
  std::string hsform();
  bool nochild();
  void setstretch(Child *cc, std::string factor);
  Child *setmenuid(std::string id);
  void setpn(std::string p);
  void showit();
  void signalevent(Child *c);
  bool split(std::string c, std::string s);
  void splitend();
  std::string state(int evt);

  Form *pform;
  std::string event;
  std::string lasttype;
  std::string locale;
  std::string sysdata;
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
