
#include <QScrollBar>

#include "../base/state.h"
#include "wd.h"
#include "scrollbar.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// optional parms are:
// "v" - vertical
// minimum
// single step
// page step
// maximum
// position

// ---------------------------------------------------------------------
ScrollBar::ScrollBar(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="scrollbar";
  QScrollBar *w=new QScrollBar(Qt::Horizontal);
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("v")),defChildStyle);
  if (unopt.size() && !qsnumeric(unopt)) {
    error("unrecognized child style: " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
#ifdef QT_OS_ANDROID
  w->setStyleSheet(scrollbarstyle(config.ScrollBarSize*DM_density));
#endif

  int i=0;
  if ((i<opt.size()) && (opt.at(i)=="v")) {
    w->setOrientation(Qt::Vertical);
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
void ScrollBar::valueChanged()
{
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void ScrollBar::set(string p,string v)
{
  QScrollBar *w=(QScrollBar*) widget;
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
  else if (cmd=="step")
    w->setSingleStep(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="page")
    w->setPageStep(c_strtoi(q2s(arg.at(0))));
  else if (cmd=="pos"|| cmd=="value")
    w->setSliderPosition(c_strtoi(v));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string ScrollBar::state()
{
  QScrollBar *w=(QScrollBar*) widget;
  return spair(id,i2s(w->sliderPosition()));
}
