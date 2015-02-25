
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
  type = "scrollarea";
  QScrollArea *w = new QScrollArea;
#ifdef QT_OS_ANDROID
  w->setStyleSheet(scrollbarstyle(config.ScrollBarSize*DM_density));
#endif
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  w->setObjectName(qn);
  childStyle(opt);
  view=f->children.last();
  w->setWidget(view->widget);
  w->setWidgetResizable(true);
  p->layout->removeWidget(view->widget);
}

// ---------------------------------------------------------------------
// set horizontal, vertical scrollbar positions
// ignore any -1
void ScrollArea::set(string p,string s)
{
  QScrollArea *w=(QScrollArea*) widget;
  QScrollBar *h=w->horizontalScrollBar();
  QScrollBar *v=w->verticalScrollBar();
  QStringList opt=qsplit(s);

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
        v->setSliderPosition(n);
    }
  } else Child::set(p,s);
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

