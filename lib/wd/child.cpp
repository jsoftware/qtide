
#include "wd.h"
#include "child.h"
#include "form.h"
#include "font.h"

// ---------------------------------------------------------------------
Child::Child(string s, string p, Form *f)
{
  grouped=false;
  id=s;
  parms=p;
  pform=f;
  locale="";
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
void Child::set(string p)
{
  Q_UNUSED(p);
}

// ---------------------------------------------------------------------
void Child::setenable(string p)
{
  Q_UNUSED(p);
}

// ---------------------------------------------------------------------
void Child::setp(string p, string v)
{
  if (!widget) return;
  if (p=="locale")
    locale=remquotes(v);
  else if (p=="focus")
    widget->setFocus();
  else if (p=="font")
    setfont((Font(v)).font);
  else if (p=="invalid")
    widget->update();
  else if (p=="show")
    widget->setVisible(!(remquotes(v)=="0" || remquotes(v)==""));
  else if (p=="xywh")
    setxywh(v,2);
  else if (p=="xywhx")
    setxywh(v,1);

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
string Child::state()
{
  return"";
}

// ---------------------------------------------------------------------
void Child::setxywh(string p,int unit)
{
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=4) {
    qDebug() << "setxywh requires 4 numbers: " + s2q(p);
  } else {
    widget->resize(n.at(2).toInt()*unit,n.at(3).toInt()*unit);
  }
}

// ---------------------------------------------------------------------
void Child::setminwh(int w, int h)
{
  if (widget && w && h) {
#ifdef ANDROID
    if (! ((type=="isigraph")||(type=="listbox")||(type=="editm")||(type=="webview"))) return;
#endif
    widget->resize(w,h);
    widget->setMinimumSize(w,h);
    widget->updateGeometry();
  }
}

