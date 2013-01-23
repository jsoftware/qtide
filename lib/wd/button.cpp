
#include <QPushButton>
#include <QSignalMapper>

#include "wd.h"
#include "button.h"
#include "form.h"

// ---------------------------------------------------------------------
Button::Button(string s, string p, Form *f) : Child(s,p,f)
{
  type="button";
  QPushButton *w=new QPushButton;
  widget=(QWidget*) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
  w->setText(qs);
  connect(w,SIGNAL(clicked()),f->signalMapper,SLOT(map()));
  f->signalMapper->setMapping(w,(QWidget*)this);
}

// ---------------------------------------------------------------------
void Button::setp(string p,string v)
{
  if (p=="caption")
    ((QPushButton *)widget)->setText(s2q(remquotes(v)));
  else Child::setp(p,v);
}
