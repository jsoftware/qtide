
#include <QProgressBar>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "progressbar.h"
#include "wd.h"

// ---------------------------------------------------------------------
ProgressBar::ProgressBar(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="progressbar";
  QProgressBar *w=new QProgressBar;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);

  int i=0;
  if ((i<opt.size()) && (opt.at(i)=="v")) {
    i++;
    w->setOrientation(Qt::Vertical);
  }
  if (i<opt.size()) {
    w->setMinimum(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setMaximum(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setValue(c_strtoi(q2s(opt.at(i))));
    i++;
  }
}

// ---------------------------------------------------------------------
std::string ProgressBar::get(std::string p,std::string v)
{
  QProgressBar *w=(QProgressBar*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("max")+"\012"+ "min"+"\012"+ "pos"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="min")
    r=i2s(w->minimum());
  else if (p=="max")
    r=i2s(w->maximum());
  else if (p=="pos"|| p=="value")
    r=i2s(w->value());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void ProgressBar::set(std::string p,std::string v)
{
  QProgressBar *w=(QProgressBar*) widget;
  QString cmd=s2q(p);
  QStringList arg=qsplit(v);
  if (arg.isEmpty()) {
    Child::set(p,v);
    return;
  }
  if (cmd=="min")
    w->setMinimum(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="max")
    w->setMaximum(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="pos" || cmd=="value")
    w->setValue(c_strtoi((v)));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string ProgressBar::state()
{
  QProgressBar *w=(QProgressBar*) widget;
  return spair(id,i2s(w->value()));
}
