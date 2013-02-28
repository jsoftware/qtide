
#include <QLabel>
#include <QStatusBar>

#include "wd.h"
#include "statusbar.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
StatusBar::StatusBar(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="StatusBar";
  QStatusBar *w=new QStatusBar(p);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
}

// ---------------------------------------------------------------------
void StatusBar::setp(string p,string v)
{
  QStatusBar *w=(QStatusBar*) widget;
  QStringList arg=qsplit(v);

  int m=0;
  int n=arg.size();
  QString s;

  if (n>0)
    s=arg.at(0);
  if (n>1)
    m=arg.at(1).toInt();

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
      info("Statusbar","setlabel needs label id and text");
      return;
    }
    m=labels.indexOf(s);
    if (m<0) {
      info("Statusbar","label not found: " + s);
      return;
    }
    labelw.at(m)->setText(arg.at(1));
  } else Child::setp(p,v);
}

