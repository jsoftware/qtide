
#include <QSlider>

#include "wd.h"
#include "slider.h"
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
Slider::Slider(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="Slider";
  QSlider *w=new QSlider(Qt::Horizontal,p);
  QString qn=s2q(n);
  widget=(QWidget*) w;
  w->setObjectName(qn);
  QStringList opt=qsplit(s);

  int i=0;
  if ((i<opt.size()) && (opt.at(i)=="v")) {
    w->setOrientation(Qt::Vertical);
    i++;
  }
  if (i<opt.size()) {
    w->setTickPosition((QSlider::TickPosition)c_strtoi(q2s(opt.at(i))));
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
void Slider::valueChanged()
{
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Slider::set(string p,string v)
{
  QSlider *w=(QSlider*) widget;
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
  else if (cmd=="tic")
    w->setTickPosition((QSlider::TickPosition)(c_strtoi(q2s(arg.at(0)))));
  else if (cmd=="step")
    w->setSingleStep(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="page")
    w->setPageStep(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="pos"|| cmd=="value")
    w->setSliderPosition(c_strtoi(v));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string Slider::state()
{
  QSlider *w=(QSlider*) widget;
  return spair(id,i2s(w->sliderPosition()));
}
