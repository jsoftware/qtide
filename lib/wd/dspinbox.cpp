
#include <QDoubleSpinBox>

#include "wd.h"
#include "dspinbox.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// optional parms are:
// decimal places
// minimum
// single step
// maximum
// value

// ---------------------------------------------------------------------
DSpinBox::DSpinBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dspinbox";
  QDoubleSpinBox *w=new QDoubleSpinBox;
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size() && !qsnumeric(unopt)) {
    error("unrecognized child style: " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  w->setLocale(QLocale::C);

  int i=0;
  if (i<opt.size()) {
    w->setDecimals(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setMinimum(c_strtod(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setSingleStep(c_strtod(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setMaximum(c_strtod(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setValue(c_strtod(q2s(opt.at(i))));
    i++;
  }
  connect(w,SIGNAL(valueChanged(double)),
          this,SLOT(valueChanged()));
}

// ---------------------------------------------------------------------
void DSpinBox::valueChanged()
{
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void DSpinBox::set(string p,string v)
{
  QDoubleSpinBox *w=(QDoubleSpinBox*) widget;
  QString cmd=s2q(p);
  QStringList arg=qsplit(v);
  if (arg.isEmpty()) {
    Child::set(p,v);
    return;
  }
  if (cmd=="decimals")
    w->setDecimals(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="min")
    w->setMinimum(c_strtod(q2s(arg.at(0))));
  else if (cmd=="max")
    w->setMaximum(c_strtod(q2s(arg.at(0))));
  else if (p=="readonly")
    w->setReadOnly(remquotes(v)!="0");
  else if (cmd=="step")
    w->setSingleStep(c_strtod(q2s(arg.at(0))));
  else if (cmd=="value")
    w->setValue(c_strtod(v));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string DSpinBox::state()
{
  QDoubleSpinBox *w=(QDoubleSpinBox*) widget;
  return spair(id,d2s(w->value()));
}
