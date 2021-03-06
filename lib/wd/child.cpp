
#include "child.h"
#include "font.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

extern Form *form;

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
  if (widget) {
#ifndef QT_NO_WEBENGINE
    if (type!="webview") delete widget;
#else
    delete widget;
#endif
  }
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
string Child::get(string p,string v)
{
  string r="";
  if (v.size() && p!="extent") {
    error("extra parameters: " + id + " " + p + " " + v);
    return "";
  }
  if (p=="property") {
    r+=string("enable")+"\012"+ "extent"+"\012";
    r+=string("focuspolicy")+"\012"+ "font"+"\012"+ "hasfocus"+"\012"+ "hwnd"+"\012";
    r+=string("id")+"\012";
    r+=string("maxwh")+"\012"+ "minwh"+"\012"+ "nextfocus"+"\012"+ "parent"+"\012"+ "prevfocus"+"\012";
    r+=string("property")+"\012"+ "sizepolicy"+"\012"+ "state"+"\012"+ "stylesheet"+"\012";
    r+=string("tooltip")+"\012"+ "type"+"\012"+ "visible"+"\012"+ "wh"+"\012"+ "xywh"+"\012";
  } else if (p=="enable") {
    if (widget) r=i2s(widget->isEnabled());
  } else if (p=="extent") {
    if (widget) {
      QFontMetrics fm = QFontMetrics(widget->font());
      r=i2s(fm.width(s2q(v)))+" "+i2s(fm.height());
    }
  } else if (p=="focuspolicy") {
    if (widget) {
      int f=widget->focusPolicy();
      if (f==Qt::TabFocus)
        r="tab";
      else if (f==Qt::ClickFocus)
        r="click";
      else if (f==Qt::StrongFocus)
        r="strong";
      else if (f==Qt::NoFocus)
        r="no";
      else
        r="unknown";
    }
  } else if (p=="font") {
    if (widget) r=q2s(fontspec(widget->font()));
  } else if (p=="hasfocus") {
    if (widget) r=i2s(widget->hasFocus());
  } else if (p=="hwnd") {
    r=p2s(this);
  } else if (p=="id") {
    r=id;
  } else if (p=="maxwh") {
    if (widget) {
      QSize size=widget->maximumSize();
      r=i2s(size.width())+" "+i2s(size.height());
    }
  } else if (p=="minwh") {
    if (widget) {
      QSize size=widget->minimumSize();
      r=i2s(size.width())+" "+i2s(size.height());
    }
  } else if (p=="nextfocus") {
    if (widget) {
      r=getfocuschain(false);
    }
  } else if (p=="parent") {
    r=pform->id;
  } else if (p=="prevfocus") {
    if (widget) {
      r=getfocuschain(true);
    }
  } else if (p=="sizepolicy") {
    if (widget) {
      string h,vr;
      int hoz=widget->sizePolicy().horizontalPolicy();
      if (hoz==QSizePolicy::Fixed)
        h="fixed";
      else if (hoz==QSizePolicy::Minimum)
        h="minimum";
      else if (hoz==QSizePolicy::Maximum)
        h="maximum";
      else if (hoz==QSizePolicy::Preferred)
        h="preferred";
      else if (hoz==QSizePolicy::Expanding)
        h="expanding";
      else if (hoz==QSizePolicy::MinimumExpanding)
        h="minimumexpanding";
      else if (hoz==QSizePolicy::Ignored)
        h="ignored";
      else
        h="unknown";
      int ver=widget->sizePolicy().verticalPolicy();
      if (ver==QSizePolicy::Fixed)
        vr="fixed";
      else if (ver==QSizePolicy::Minimum)
        vr="minimum";
      else if (ver==QSizePolicy::Maximum)
        vr="maximum";
      else if (ver==QSizePolicy::Preferred)
        vr="preferred";
      else if (ver==QSizePolicy::Expanding)
        vr="expanding";
      else if (ver==QSizePolicy::MinimumExpanding)
        vr="minimumexpanding";
      else if (ver==QSizePolicy::Ignored)
        vr="ignored";
      else
        vr="unknown";
      r=h + " " + vr;
    }
  } else if (p=="state") {
    r=state();
  } else if (p=="stylesheet") {
    if (widget) r=q2s(widget->styleSheet());
  } else if (p=="tooltip") {
    if (widget) r=q2s(widget->toolTip());
  } else if (p=="type") {
    r=type;
  } else if (p=="visible") {
    if (widget) r=i2s(widget->isVisible());
  } else if (p=="wh") {
    if (widget) {
      QSize size=widget->size();
      r=i2s(size.width())+" "+i2s(size.height());
    }
  } else if (p=="xywh") {
    if (widget) {
      QPoint pos=widget->mapTo(widget->window(),widget->pos());
      QSize size=widget->size();
      r=i2s(pos.x())+" "+i2s(pos.y())+" "+i2s(size.width())+" "+i2s(size.height());
    }
  } else
    error("get command not recognized: " + id + " " + p + " " + v);
  return r;
}

// ---------------------------------------------------------------------
string Child::getfocuschain(bool prev)
{
  if (!widget) return "";
  QWidget *w=(prev)?widget->previousInFocusChain():widget->nextInFocusChain();
  if (!w) return "";
  for (int i=pform->children.size()-1; 0<=i; i--) {
    QWidget *c;
    if ((c=pform->children.at(i)->widget) && (w==c || c->isAncestorOf(w)))
      return pform->children.at(i)->id;
  }
  return"";
}

// ---------------------------------------------------------------------
string Child::getsysdata()
{
  return"";
}

// ---------------------------------------------------------------------
void Child::set(string p,string v)
{
  if (p=="cursor") {
    if (widget) {
      int a=c_strtoi(v);
      if (-1==a)
        widget->unsetCursor();
      else
        widget->setCursor((Qt::CursorShape)a);
    }
  } else if (p=="enable") {
    if (widget) widget->setEnabled(remquotes(v)!="0");
  } else if (p=="focus") {
    if (widget) widget->setFocus();
  } else if (p=="focuspolicy") {
    setfocuspolicy(v);
  } else if (p=="font") {
    if (widget) widget->setFont((Font(v)).font);
  } else if (p=="invalid") {
    if (widget) widget->update();
  } else if (p=="nofocus") {
    if (widget) widget->setFocusPolicy(Qt::NoFocus);
  } else if (p=="show"||p=="visible") {
    if (widget) widget->setVisible(remquotes(v)!="0");
  } else if (p=="sizepolicy") {
    setsizepolicy(v);
  } else if (p=="stylesheet") {
    if (widget) widget->setStyleSheet(s2q(remquotes(v)));
  } else if (p=="tooltip") {
    if (widget) widget->setToolTip(s2q(remquotes(v)));
  } else if (p=="wh") {
    setwh(v);
  } else if (p=="maxwh") {
    setmaxwhv(v);
  } else if (p=="minwh") {
    setminwhv(v);
  } else
    error("set command not recognized: " + id + " " + p + " " + v);
}

// ---------------------------------------------------------------------
void Child::setfocuspolicy(string p)
{
  if (widget) wdsetfocuspolicy(widget,p);
}

// ---------------------------------------------------------------------
void Child::setform()
{
  form=pform;
  form->child=this;
}

// ---------------------------------------------------------------------
void Child::setsizepolicy(string p)
{
  if (widget) wdsetsizepolicy(widget,p);
}

// ---------------------------------------------------------------------
void Child::setwh(string p)
{
  if (widget) wdsetwh(widget,p);
}

// ---------------------------------------------------------------------
void Child::setmaxwhv(string p)
{
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=2) {
    error("set maxwh requires 2 numbers: " + id + " " + p);
  } else {
    int w=c_strtoi(q2s(n.at(0)));
    int h=c_strtoi(q2s(n.at(1)));
    setmaxwh(w,h);
  }
}

// ---------------------------------------------------------------------
void Child::setmaxwh(int w, int h)
{
  if (widget && w && h) {
    widget->setMaximumSize(w,h);
    widget->updateGeometry();
  }
}

// ---------------------------------------------------------------------
void Child::setminwhv(string p)
{
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=2) {
    error("set minwh requires 2 numbers: " + id + " " + p);
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
