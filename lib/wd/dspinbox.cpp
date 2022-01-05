
#include <QDoubleSpinBox>

#include "cmd.h"
#include "dspinbox.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

// optional parms are:
// decimal places
// minimum
// single step
// maximum
// value

// ---------------------------------------------------------------------
DSpinBox::DSpinBox(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dspinbox";
  QDoubleSpinBox *w=new QDoubleSpinBox;
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"")) return;
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
std::string DSpinBox::get(std::string p,std::string v)
{
  QDoubleSpinBox *w=(QDoubleSpinBox*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("decimals")+"\012"+ "max"+"\012"+ "min"+"\012"+ "readonly"+"\012"+ "step"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="min")
    r=i2s(w->minimum());
  else if (p=="max")
    r=i2s(w->maximum());
  else if (p=="step")
    r=i2s(w->singleStep());
  else if (p=="decimals")
    r=i2s(w->decimals());
  else if (p=="readonly")
    r=i2s(w->isReadOnly());
  else if (p=="value")
    r=d2s(w->value());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void DSpinBox::set(std::string p,std::string v)
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
std::string DSpinBox::state()
{
  QDoubleSpinBox *w=(QDoubleSpinBox*) widget;
  return spair(id,d2s(w->value()));
}
