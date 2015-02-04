
#include <QPushButton>
#include <QSignalMapper>

#include "wd.h"
#include "button.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Button::Button(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="button";
  QPushButton *w=new QPushButton;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  w->setObjectName(qn);
  w->setText(qn);
  if (opt.contains("default"))
    w->setAutoDefault(true);
  connect(w,SIGNAL(clicked()),f->signalMapper,SLOT(map()));
  f->signalMapper->setMapping(w,(QWidget*)this);
}

// ---------------------------------------------------------------------
void Button::set(string p,string v)
{
  if (p=="caption" || p=="text")
    ((QPushButton *)widget)->setText(s2q(remquotes(v)));
  else if (p=="icon")
    ((QPushButton *)widget)->setIcon(QIcon(s2q(remquotes(v))));
  else Child::set(p,v);
}
