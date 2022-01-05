
#include <QSpinBox>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "spinbox.h"
#include "wd.h"

// optional parms are:
// minimum
// single step
// maximum
// value

// ---------------------------------------------------------------------
SpinBox::SpinBox(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="spinbox";
  QSpinBox *w=new QSpinBox;
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);
  w->setLocale(QLocale::C);

  int i=0;
  if (i<opt.size()) {
    w->setMinimum(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setSingleStep(c_strtoi(q2s(opt.at(i))));
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
  connect(w,SIGNAL(valueChanged(int)),
          this,SLOT(valueChanged()));
}

// ---------------------------------------------------------------------
void SpinBox::valueChanged()
{
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
std::string SpinBox::get(std::string p,std::string v)
{
  QSpinBox *w=(QSpinBox*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("max")+"\012"+ "min"+"\012"+ "readonly"+"\012"+ "step"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="min")
    r=i2s(w->minimum());
  else if (p=="max")
    r=i2s(w->maximum());
  else if (p=="step")
    r=i2s(w->singleStep());
  else if (p=="readonly")
    r=i2s(w->isReadOnly());
  else if (p=="value")
    r=i2s(w->value());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void SpinBox::set(std::string p,std::string v)
{
  QSpinBox *w=(QSpinBox*) widget;
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
  else if (p=="readonly")
    w->setReadOnly(remquotes(v)!="0");
  else if (cmd=="step")
    w->setSingleStep(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="value")
    w->setValue(c_strtoi(v));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string SpinBox::state()
{
  QSpinBox *w=(QSpinBox*) widget;
  return spair(id,i2s(w->value()));
}
