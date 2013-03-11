
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
  else if (p=="font")
    setfont((Font(v)).font);
  else if (p=="invalid")
    widget->update();
  else if (p=="show")
    widget->setVisible(remquotes(v)!="0");
  else if (p=="stylesheet")
    setstylesheet(v);
  else if (p=="xywh")
    setxywh(v,2);
  else if (p=="xywhx")
    setxywh(v,1);
  else
    error("set command not recognized: " + p);
}

// ---------------------------------------------------------------------
void Child::setfont(QFont f)
{
  if (widget) widget->setFont(f);
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
void Child::setxywh(string p,int unit)
{
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=4) {
    qDebug() << "setxywh requires 4 numbers: " + s2q(p);
  } else {
    widget->resize(c_strtoi(q2s(n.at(2)))*unit,c_strtoi(q2s(n.at(3)))*unit);
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
