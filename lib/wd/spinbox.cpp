
#include <QSpinBox>

#include "wd.h"
#include "spinbox.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// optional parms are:
// "v" - vertical
// tick placement
// minimum
// single step
// page step
// maximum
// position

// ---------------------------------------------------------------------
SpinBox::SpinBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="SpinBox";
  QSpinBox *w=new QSpinBox(p);
  QString qn=s2q(n);
  widget=(QWidget*) w;
  w->setObjectName(qn);
  QStringList opt=qsplit(s);

  int i=0;
  if (i<opt.size()) {
    w->setMinimum(opt.at(i).toInt());
    i++;
  }
  if (i<opt.size()) {
    w->setSingleStep(opt.at(i).toInt());
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
    w->setMinimum(arg.at(0).toInt());
  else if (cmd=="max")
    w->setMaximum(arg.at(0).toInt());
  else if (cmd=="value")
    w->setValue(s2q(v).toInt());
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string SpinBox::state()
{
  QSpinBox *w=(QSpinBox*) widget;
  return spair(id,i2s(w->value()));
}
