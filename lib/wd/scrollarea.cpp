
#include <QScrollArea>
#include <QScrollBar>

#include "../base/state.h"
#include "wd.h"
#include "scrollarea.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// the scrollbars are in pixels and vary as the form is resized
// minimum is 0, single step is 1, page step is 10
// the state shows position and maximum
// the h,v position can be set with wd 'set id pos h p'

// ---------------------------------------------------------------------
ScrollArea::ScrollArea(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="scrollarea";
  QScrollArea *w = new QScrollArea;
#ifdef QT_OS_ANDROID
  w->setStyleSheet(scrollbarstyle(config.ScrollBarSize*DM_density));
#endif
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  view=f->children.last();
  w->setWidget(view->widget);
  w->setWidgetResizable(true);
  p->layout->removeWidget(view->widget);
}

// ---------------------------------------------------------------------
ScrollArea::~ScrollArea()
{
  if (view && view->widget) {
    ((QScrollArea *)widget)->takeWidget();
    delete view->widget;
    view->widget=0;
  }
  if (widget) delete (QScrollArea *)widget;
  widget=0;
}

// ---------------------------------------------------------------------
string ScrollArea::get(string p,string v)
{
  QScrollArea *w=(QScrollArea*) widget;
  QScrollBar *h=w->horizontalScrollBar();
  QScrollBar *q=w->verticalScrollBar();
  string r;
  if (p=="property") {
    r+=string("max")+"\012"+ "min"+"\012"+ "pos"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="min")
    r=i2s(h->minimum())+" "+i2s(q->minimum());
  else if (p=="max")
    r=i2s(h->maximum())+" "+i2s(q->maximum());
  else if (p=="pos"|| p=="value")
    r=i2s(h->sliderPosition())+" "+i2s(q->sliderPosition());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
// set horizontal, vertical scrollbar positions
// ignore any -1
void ScrollArea::set(string p,string v)
{
  QScrollArea *w=(QScrollArea*) widget;
  QScrollBar *h=w->horizontalScrollBar();
  QScrollBar *q=w->verticalScrollBar();
  QStringList opt=qsplit(v);

  if (p=="pos") {
    int n;
    if (opt.size()) {
      n=c_strtoi(q2s(opt.at(0)));
      if (n>-1)
        h->setSliderPosition(n);
    }
    if (1<opt.size()) {
      n=c_strtoi(q2s(opt.at(1)));
      if (n>-1)
        q->setSliderPosition(n);
    }
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
// shows horizontal position, maximum
// followed by vertical position, maximum
string ScrollArea::state()
{
  QScrollArea *w=(QScrollArea*) widget;
  QScrollBar *h=w->horizontalScrollBar();
  QScrollBar *v=w->verticalScrollBar();
  return spair(id,i2s(h->sliderPosition())+" "+i2s(h->maximum())+
               " "+i2s(v->sliderPosition())+" "+i2s(v->maximum()));
}

