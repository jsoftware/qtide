
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
  QSlider *w=new QSlider(Qt::Horizontal);
  QString qn=s2q(n);
  widget=(QWidget*) w;
  w->setObjectName(qn);
  QStringList opt=qsplit(s);

  int i=0;
  if (opt.at(i)=="v") {
    i++;
    w->setOrientation(Qt::Vertical);
  }
  if (i<opt.size()) {
    w->setTickPosition((QSlider::TickPosition)opt.at(i).toInt());
    i++;
  }
  if (i<opt.size()) {
    w->setMinimum(opt.at(i).toInt());
    i++;
  }
  if (i<opt.size()) {
    w->setSingleStep(opt.at(i).toInt());
    i++;
  }
  if (i<opt.size()) {
    w->setPageStep(opt.at(i).toInt());
    i++;
  }
  if (i<opt.size()) {
    w->setMaximum(opt.at(i).toInt());
    i++;
  }
  if (i<opt.size()) {
    w->setSliderPosition(opt.at(i).toInt());
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
void Slider::set(string p)
{
  QSlider *w=(QSlider*) widget;
  w->setSliderPosition(s2q(p).toInt());
}

// ---------------------------------------------------------------------
void Slider::setp(string p,string v)
{
  QSlider *w=(QSlider*) widget;
  QString cmd=s2q(p);
  QStringList arg=qsplit(v);
  if (arg.isEmpty()) return;
  if (cmd=="min")
    w->setMinimum(arg.at(0).toInt());
  else if (cmd=="max")
    w->setMaximum(arg.at(0).toInt());
  else Child::setp(p,v);
}

// ---------------------------------------------------------------------
string Slider::state()
{
  QSlider *w=(QSlider*) widget;
  return spair(id,i2s(w->sliderPosition()));
}
