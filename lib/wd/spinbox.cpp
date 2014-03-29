
#include <QSpinBox>

#include "wd.h"
#include "spinbox.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// optional parms are:
// minimum
// single step
// maximum
// value

// ---------------------------------------------------------------------
SpinBox::SpinBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="SpinBox";
  QSpinBox *w=new QSpinBox;
  QString qn=s2q(n);
  widget=(QWidget*) w;
  w->setObjectName(qn);
  QStringList opt=qsplit(s);

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
void SpinBox::set(string p,string v)
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
string SpinBox::state()
{
  QSpinBox *w=(QSpinBox*) widget;
  return spair(id,i2s(w->value()));
}
