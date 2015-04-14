
#include <QBoxLayout>
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
  QLabel *w=new QLabel;
  widget=(QWidget *) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("staticbox left right center sunken raised panel")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  w->setWordWrap(true);
  if (s.substr(0,9)!="staticbox")
    w->setText(qn);
  if (opt.contains("left"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  else if (opt.contains("right"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  else if (opt.contains("center"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
  if (opt.contains("sunken")) {
    w->setFrameStyle(QFrame::Sunken|QFrame::Panel);
    w->setMargin(4);
  } else if (opt.contains("raised")) {
    w->setFrameStyle(QFrame::Raised|QFrame::Panel);
    w->setMargin(4);
  } else if (opt.contains("panel")) {
    w->setFrameStyle(QFrame::Panel);
    w->setMargin(4);
  }
}

// ---------------------------------------------------------------------
string Static::get(string p,string v)
{
  QLabel *w=(QLabel*) widget;
  string r;
  if (p=="property") {
    r+=string("caption")+"\012"+ "text"+"\012";
    r+=Child::get(p,v);
  } else if (p=="caption"||p=="text")
    r=q2s(w->text());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Static::set(string p,string v)
{
  if (p=="caption" || p=="text")
    ((QLabel *)widget)->setText(s2q(remquotes(v)));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string Static::state()
{
  return "";
}
