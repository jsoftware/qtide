
#include <QProgressBar>

#include "wd.h"
#include "progressbar.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
ProgressBar::ProgressBar(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="ProgressBar";
  QProgressBar *w=new QProgressBar(p);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  QStringList opt=qsplit(s);
  int i=0;
  if (opt.at(i)=="v") {
    i++;
    w->setOrientation(Qt::Vertical);
  }
  if (i<opt.size()) {
    w->setMinimum(opt.at(i).toInt());
    i++;
  }
  if (i<opt.size()) {
    w->setMaximum(opt.at(i).toInt());
    i++;
  }
  if (i<opt.size()) {
    w->setValue(opt.at(i).toInt());
    i++;
  }
}

// ---------------------------------------------------------------------
void ProgressBar::set(string p)
{
  QProgressBar *w=(QProgressBar*) widget;
  w->setValue(s2q(p).toInt());
}

// ---------------------------------------------------------------------
void ProgressBar::setp(string p,string v)
{
  QProgressBar *w=(QProgressBar*) widget;
  QString cmd=s2q(p);
  QStringList arg=qsplit(v);
  if (arg.isEmpty()) return;
  if (cmd=="min")
    w->setMinimum(arg.at(0).toInt());
  else if (cmd=="max")
    w->setMaximum(arg.at(0).toInt());
  else Child::setp(p,v);
}

