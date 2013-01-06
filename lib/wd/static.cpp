
#include <QLabel>

#include "wd.h"
#include "static.h"
#include "form.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Static::Static(string s, string p, Form *f) : Child(s,p,f)
{
  type="static";
  QLabel *w=new QLabel;
  widget=(QWidget *) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
  w->setWordWrap(true);
  w->setText(qs);
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

