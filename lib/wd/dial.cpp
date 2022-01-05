
#include <QDial>

#include "cmd.h"
#include "dial.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

// optional parms are:
// "v" - notchesvisible
// "w" - wrapping
// minimum
// single step
// page step
// maximum
// position

// ---------------------------------------------------------------------
Dial::Dial(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dial";
  QDial *w=new QDial();
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"v w")) return;
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
std::string Dial::get(std::string p,std::string v)
{
  QDial *w=(QDial*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("max")+"\012"+ "min"+"\012"+ "notchesvisible"+"\012"+ "page"+"\012"+ "pos"+"\012"+ "step"+"\012"+ "wrap"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="min")
    r=i2s(w->minimum());
  else if (p=="max")
    r=i2s(w->maximum());
  else if (p=="wrap")
    r=i2s(w->wrapping());
  else if (p=="notchesvisible")
    r=i2s(w->notchesVisible());
  else if (p=="step")
    r=i2s(w->singleStep());
  else if (p=="page")
    r=i2s(w->pageStep());
  else if (p=="pos"|| p=="value")
    r=i2s(w->sliderPosition());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Dial::set(std::string p,std::string v)
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
std::string Dial::state()
{
  QDial *w=(QDial*) widget;
  return spair(id,i2s(w->sliderPosition()));
}
