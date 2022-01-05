
#include <QSlider>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "slider.h"
#include "wd.h"

// optional parms are:
// "v" - vertical
// tick placement
// minimum
// single step
// page step
// maximum
// position

// ---------------------------------------------------------------------
Slider::Slider(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="slider";
  QSlider *w=new QSlider(Qt::Horizontal);
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"v")) return;
  w->setObjectName(qn);
  childStyle(opt);

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
std::string Slider::get(std::string p,std::string v)
{
  QSlider *w=(QSlider*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("max")+"\012"+ "min"+"\012"+ "page"+"\012"+ "pos"+"\012"+ "step"+"\012"+ "tic"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="min")
    r=i2s(w->minimum());
  else if (p=="max")
    r=i2s(w->maximum());
  else if (p=="tic")
    r=i2s(w->tickPosition());
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
void Slider::set(std::string p,std::string v)
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
std::string Slider::state()
{
  QSlider *w=(QSlider*) widget;
  return spair(id,i2s(w->sliderPosition()));
}
