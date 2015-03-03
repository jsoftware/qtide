
#include "wd.h"
#include "child.h"
#include "form.h"
#include "pane.h"
#include "font.h"

// ---------------------------------------------------------------------
Child::Child(string n, string s, Form *f, Pane *p)
{
  grouped=false;
  id=n;
  eid=n;
  parms=s;
  pform=f;
  ppane=p;
  widget=0;
  locale="";
}

// ---------------------------------------------------------------------
Child::~Child()
{
  if (widget) delete widget;
  widget=0;
}

// ---------------------------------------------------------------------
void Child::childStyle(QStringList opt)
{
  Q_UNUSED(opt);
}

// ---------------------------------------------------------------------
void Child::cmd(string p, string v)
{
  Q_UNUSED(p);
  Q_UNUSED(v);
}

// ---------------------------------------------------------------------
string Child::get(string p, string v)
{
  Q_UNUSED(p);
  Q_UNUSED(v);
  return"";
}

// ---------------------------------------------------------------------
string Child::getsysdata()
{
  return"";
}

// ---------------------------------------------------------------------
void Child::set(string p, string v)
{
  if (p=="enable") {
    if (widget) widget->setEnabled(remquotes(v)!="0");
  } else if (p=="locale") {
    locale=remquotes(v);
  } else if (p=="focus") {
    if (widget) widget->setFocus();
  } else if (p=="focuspolicy") {
    setfocuspolicy(v);
  } else if (p=="font") {
    setfont((Font(v)).font);
  } else if (p=="invalid") {
    if (widget) widget->update();
  } else if (p=="nofocus") {
    if (widget) widget->setFocusPolicy(Qt::NoFocus);
  } else if (p=="show") {
    if (widget) widget->setVisible(remquotes(v)!="0");
  } else if (p=="sizepolicy") {
    setsizepolicy(v);
  } else if (p=="stylesheet") {
    setstylesheet(v);
  } else if (p=="tooltip") {
    settooltip(v);
  } else if (p=="wh") {
    setwh(v);
  } else if (p=="minwh") {
    setminwhv(v);
  } else
    error("set command not recognized: " + p);
}

// ---------------------------------------------------------------------
void Child::setfont(QFont f)
{
  if (widget) widget->setFont(f);
}

// ---------------------------------------------------------------------
void Child::setfocuspolicy(string p)
{
  if (!widget) return;
  if (p=="tab")
    widget->setFocusPolicy(Qt::TabFocus);
  else if (p=="click")
    widget->setFocusPolicy(Qt::ClickFocus);
  else if (p=="strong")
    widget->setFocusPolicy(Qt::StrongFocus);
  else if (p=="no")
    widget->setFocusPolicy(Qt::NoFocus);
  else error("set focuspolicy requires option to be tab,click,strong or no: " + p);
}

// ---------------------------------------------------------------------
void Child::setform()
{
  form=pform;
}

// enum QSizePolicy::Policy
//
// This enum describes the various per-dimension sizing types used when constructing a QSizePolicy.
//
//           Constant                Value                                   Description
//                                             The QWidget::sizeHint() is the only acceptable alternative, so the
// QSizePolicy::Fixed            0             widget can never grow or shrink (e.g. the vertical direction of a push
//                                             button).
//                                             The sizeHint() is minimal, and sufficient. The widget can be expanded,
// QSizePolicy::Minimum          GrowFlag      but there is no advantage to it being larger (e.g. the horizontal
//                                             direction of a push button). It cannot be smaller than the size
//                                             provided by sizeHint().
//                                             The sizeHint() is a maximum. The widget can be shrunk any amount
// QSizePolicy::Maximum          ShrinkFlag    without detriment if other widgets need the space (e.g. a separator
//                                             line). It cannot be larger than the size provided by sizeHint().
//                               GrowFlag |    The sizeHint() is best, but the widget can be shrunk and still be
// QSizePolicy::Preferred        ShrinkFlag    useful. The widget can be expanded, but there is no advantage to it
//                                             being larger than sizeHint() (the default QWidget policy).
//                               GrowFlag |    The sizeHint() is a sensible size, but the widget can be shrunk and
// QSizePolicy::Expanding        ShrinkFlag |  still be useful. The widget can make use of extra space, so it should
//                               ExpandFlag    get as much space as possible (e.g. the horizontal direction of a
//                                             horizontal slider).
//                               GrowFlag |    The sizeHint() is minimal, and sufficient. The widget can make use of
// QSizePolicy::MinimumExpanding ExpandFlag    extra space, so it should get as much space as possible (e.g. the
//                                             horizontal direction of a horizontal slider).
//                               ShrinkFlag |  The sizeHint() is ignored. The widget will get as much space as
// QSizePolicy::Ignored          GrowFlag |    possible.
//                               IgnoreFlag
//
// enum QSizePolicy::PolicyFlag
//
// These flags are combined together to form the various Policy values:
//
//        Constant         Value                                    Description
// QSizePolicy::GrowFlag   1     The widget can grow beyond its size hint if necessary.
// QSizePolicy::ExpandFlag 2     The widget should get as much space as possible.
// QSizePolicy::ShrinkFlag 4     The widget can shrink below its size hint if necessary.
// QSizePolicy::IgnoreFlag 8     The widget's size hint is ignored. The widget will get as much space as possible.

// ---------------------------------------------------------------------
void Child::setsizepolicy(string p)
{
  QString h,v;
  int hoz,ver;
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.empty()) {
    error("set sizepolicy requires 1 or 2 options: " + p);
    return;
  }
  if (n.size()==1)
    h=v=n.at(0);
  else {
    h=n.at(0);
    v=n.at(1);
  }
  if (h=="fixed")
    hoz=QSizePolicy::Fixed;
  else if (h=="minimum")
    hoz=QSizePolicy::Minimum;
  else if (h=="maximum")
    hoz=QSizePolicy::Maximum;
  else if (h=="preferred")
    hoz=QSizePolicy::Preferred;
  else if (h=="expanding")
    hoz=QSizePolicy::Expanding;
  else if (h=="minimumexpanding")
    hoz=QSizePolicy::MinimumExpanding;
  else if (h=="ignored")
    hoz=QSizePolicy::Ignored;
  else {
    error("set sizepolicy requires options to be fixed,minimum,maximum,preferred,expanding,minimumexpanding or ignored: " + p);
    return;
  }
  if (v=="fixed")
    ver=QSizePolicy::Fixed;
  else if (v=="minimum")
    ver=QSizePolicy::Minimum;
  else if (v=="maximum")
    ver=QSizePolicy::Maximum;
  else if (v=="preferred")
    ver=QSizePolicy::Preferred;
  else if (v=="expanding")
    ver=QSizePolicy::Expanding;
  else if (v=="minimumexpanding")
    ver=QSizePolicy::MinimumExpanding;
  else if (v=="ignored")
    ver=QSizePolicy::Ignored;
  else {
    error("set sizepolicy requires options to be fixed,minimum,maximum,preferred,expanding,minimumexpanding or ignored: " + p);
    return;
  }
  if (widget) widget->setSizePolicy((QSizePolicy::Policy)hoz,(QSizePolicy::Policy)ver);
}

// ---------------------------------------------------------------------
void Child::setstylesheet(string p)
{
  if (widget) widget->setStyleSheet(s2q(p));
}

// ---------------------------------------------------------------------
void Child::settooltip(string p)
{
  if (widget) widget->setToolTip(s2q(p));
}

// ---------------------------------------------------------------------
void Child::setwh(string p)
{
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=2) {
    error("set wh requires 2 numbers: " + p);
  } else {
    int w=c_strtoi(q2s(n.at(0)));
    int h=c_strtoi(q2s(n.at(1)));
    if (!widget) return;
    if (w!=-1 || h!=-1) {
      if (w==-1)
        widget->setFixedHeight(h);
      else if (h==-1)
        widget->setFixedWidth(w);
      else
        widget->setFixedSize(w,h);
    }
  }
}

// ---------------------------------------------------------------------
void Child::setminwhv(string p)
{
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=2) {
    error("set minwh requires 2 numbers: " + p);
  } else {
    int w=c_strtoi(q2s(n.at(0)));
    int h=c_strtoi(q2s(n.at(1)));
    setminwh(w,h);
  }
}

// ---------------------------------------------------------------------
void Child::setminwh(int w, int h)
{
  if (widget && w && h) {
    widget->setMinimumSize(w,h);
    widget->updateGeometry();
  }
}

// ---------------------------------------------------------------------
string Child::state()
{
  return"";
}
