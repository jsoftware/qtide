
#include <QLabel>

#include "wd.h"
#include "static.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Static::Static(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="static";
  QLabel *w=new QLabel(p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  w->setWordWrap(true);
  if (s.substr(0,9)!="staticbox")
    w->setText(qn);
}

// ---------------------------------------------------------------------
void Static::set(string p)
{
  ((QLabel *)widget)->setText(s2q(p));
}

// ---------------------------------------------------------------------
void Static::setp(string p,string v)
{
  if (p=="caption")
    ((QLabel *)widget)->setText(s2q(v));
  else Child::setp(p,v);
}

// ---------------------------------------------------------------------
void Static::setminwh(int w, int h)
{
  Q_UNUSED(w);
  Q_UNUSED(h);
}
