
#include <QLabel>
#include <QStatusBar>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "statusbar.h"
#include "wd.h"

// ---------------------------------------------------------------------
StatusBar::StatusBar(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="statusbar";
  QStatusBar *w=new QStatusBar;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);
}

// ---------------------------------------------------------------------
std::string StatusBar::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void StatusBar::set(std::string p,std::string v)
{
  QStatusBar *w=(QStatusBar*) widget;
  QStringList arg=qsplit(v);

  int m=0;
  int n=arg.size();
  QString s;

  if (n>0)
    s=arg.at(0);
  if (n>1)
    m=c_strtoi(q2s(arg.at(1)));

  if (p=="show" && n)
    w->showMessage(s,m);
  else if (p=="clear")
    w->clearMessage();

  else if (p=="addlabel") {
    QLabel *lab=new QLabel();
    labels.append(s);
    labelw.append(lab);
    w->addWidget(lab,0);
  } else if (p=="addlabelp") {
    QLabel *lab=new QLabel();
    labels.append(s);
    labelw.append(lab);
    w->addPermanentWidget(lab,0);
  } else if (p=="setlabel") {
    if (n<2) {
      error("setlabel needs label id and text: " + id + " " + p + " " + v);
      return;
    }
    m=labels.indexOf(s);
    if (m<0) {
      error("label not found: " + id + " " + p + " " + v);
      return;
    }
    labelw.at(m)->setText(arg.at(1));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string StatusBar::state()
{
  return "";
}
