
#include <QDial>

#include "wd.h"
#include "dial.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// optional parms are:
// "v" - notchesvisible
// "w" - wrapping
// minimum
// single step
// page step
// maximum
// position

// ---------------------------------------------------------------------
Dial::Dial(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dial";
  QDial *w=new QDial();
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("v w")),defChildStyle);
  if (unopt.size() && !qsnumeric(unopt)) {
    error("unrecognized child style: " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);

  int i=0;
  if ((i<opt.size()) && (opt.at(i)=="w"||opt.at(i)=="v")) {
    if (opt.at(i)=="w")
      w->setWrapping(true);
    else
      w->setNotchesVisible(true);
    i++;
  }
  if ((i<opt.size()) && (opt.at(i)=="w"||opt.at(i)=="v")) {
    if (opt.at(i)=="w")
      w->setWrapping(true);
    else
      w->setNotchesVisible(true);
    i++;
  }
  if (i<opt.size()) {
    w->setMinimum(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setSingleStep(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setPageStep(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setMaximum(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  if (i<opt.size()) {
    w->setSliderPosition(c_strtoi(q2s(opt.at(i))));
    i++;
  }
  connect(w,SIGNAL(valueChanged(int)),
          this,SLOT(valueChanged()));
}

// ---------------------------------------------------------------------
void Dial::valueChanged()
{
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Dial::set(string p,string v)
{
  QDial *w=(QDial*) widget;
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
  else if (cmd=="wrap")
    w->setWrapping(arg.at(0)!="0");
  else if (cmd=="notchesvisible")
    w->setNotchesVisible(arg.at(0)!="0");
  else if (cmd=="step")
    w->setSingleStep(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="page")
    w->setPageStep(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="pos"|| cmd=="value")
    w->setSliderPosition(c_strtoi(v));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string Dial::state()
{
  QDial *w=(QDial*) widget;
  return spair(id,i2s(w->sliderPosition()));
}
