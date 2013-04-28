
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
  locale="";
}

// ---------------------------------------------------------------------
void Child::cmd(string p, string v)
{
  Q_UNUSED(p);
  Q_UNUSED(v);
}

// ---------------------------------------------------------------------
void Child::get(string p, string v)
{
  Q_UNUSED(p);
  Q_UNUSED(v);
}

// ---------------------------------------------------------------------
string Child::getsysdata()
{
  return"";
}

// ---------------------------------------------------------------------
string Child::getsysmodifiers()
{
  return"";
}

// ---------------------------------------------------------------------
void Child::set(string p, string v)
{
  if (!widget) return;
  if (p=="enable")
    widget->setEnabled(remquotes(v)!="0");
  else if (p=="locale")
    locale=remquotes(v);
  else if (p=="focus")
    widget->setFocus();
  else if (p=="focuspolicy")
    setfocuspolicy(v);
  else if (p=="font")
    setfont((Font(v)).font);
  else if (p=="invalid")
    widget->update();
  else if (p=="show")
    widget->setVisible(remquotes(v)!="0");
  else if (p=="stylesheet")
    setstylesheet(v);
  else if (p=="wh")
    setwh(v,1);
  else if (p=="minwh")
    setminwhv(v);
  else
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
  if (p=="tab")
    widget->setFocusPolicy(Qt::TabFocus);
  else if (p=="click")
    widget->setFocusPolicy(Qt::ClickFocus);
  else if (p=="strong")
    widget->setFocusPolicy(Qt::StrongFocus);
  else if (p=="no")
    widget->setFocusPolicy(Qt::NoFocus);
  else error("set focuspolicy command option not recognized: " + p);
}

// ---------------------------------------------------------------------
void Child::setform()
{
  if (widget) form=pform;
}

// ---------------------------------------------------------------------
void Child::setstylesheet(string p)
{
  widget->setStyleSheet(s2q(p));
}

// ---------------------------------------------------------------------
void Child::setwh(string p,int unit)
{
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=2) {
    error("set wh requires 2 numbers: " + p);
  } else {
    widget->resize(c_strtoi(q2s(n.at(0)))*unit,c_strtoi(q2s(n.at(1)))*unit);
  }
}

// ---------------------------------------------------------------------
void Child::setminwhv(string p)
{
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=2) {
    error("set minwh requires 2 numbers: " + p);
  } else {
#ifdef ANDROID
    if (! ((type=="isigraph")||(type=="opengl")||(type=="listbox")||(type=="editm")||(type=="webview"))) return;
#endif
    int w=c_strtoi(q2s(n.at(0)));
    int h=c_strtoi(q2s(n.at(1)));
    widget->resize(w,h);
    widget->setMinimumSize(w,h);
    widget->updateGeometry();
  }
}

// ---------------------------------------------------------------------
void Child::setminwh(int w, int h)
{
  if (widget && w && h) {
#ifdef ANDROID
    if (! ((type=="isigraph")||(type=="opengl")||(type=="listbox")||(type=="editm")||(type=="webview"))) return;
#endif
    widget->resize(w,h);
    widget->setMinimumSize(w,h);
    widget->updateGeometry();
  }
}

// ---------------------------------------------------------------------
string Child::state()
{
  return"";
}
