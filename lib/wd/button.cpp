
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
  QStringList unopt=qsless(qsless(opt,qsplit("default")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  w->setText(qn);
  iconFile="";
  if (opt.contains("default"))
    w->setAutoDefault(true);
  connect(w,SIGNAL(clicked()),f->signalMapper,SLOT(map()));
  f->signalMapper->setMapping(w,(QWidget*)this);
}

// ---------------------------------------------------------------------
string Button::get(string p,string v)
{
  QPushButton *w=(QPushButton*) widget;
  string r;
  if (p=="property") {
    r+=string("caption")+"\012"+ "icon"+"\012"+ "text"+"\012";
    r+=Child::get(p,v);
  } else if (p=="caption"||p=="text")
    r=q2s(w->text());
  else if (p=="icon")
    r=iconFile;
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Button::set(string p,string v)
{
  if (p=="caption" || p=="text")
    ((QPushButton *)widget)->setText(s2q(remquotes(v)));
  else if (p=="icon") {
    iconFile=remquotes(v);
    ((QPushButton *)widget)->setIcon(QIcon(s2q(iconFile)));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
string Button::state()
{
  return "";
}
