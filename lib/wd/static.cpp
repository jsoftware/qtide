
#include <QBoxLayout>
#include <QLabel>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "static.h"
#include "wd.h"

// ---------------------------------------------------------------------
Static::Static(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="static";
  QLabel *w=new QLabel;
  widget=(QWidget *) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"staticbox left right center sunken raised panel nowrap")) return;
  if (1<(opt.contains("left")?1:0) + (opt.contains("right")?1:0) + (opt.contains("center")?1:0)) {
    error("conflicting child style: " + n + " " + q2s(opt.join(" ")));
    return;
  }
  if (1<(opt.contains("sunken")?1:0) + (opt.contains("raised")?1:0) + (opt.contains("panel")?1:0)) {
    error("conflicting child style: " + n + " " + q2s(opt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  w->setOpenExternalLinks(true);
  childStyle(opt);
  if (!opt.contains("nowrap"))
    w->setWordWrap(true);
  if (!opt.contains("staticbox"))
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
std::string Static::get(std::string p,std::string v)
{
  QLabel *w=(QLabel*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("alignment")+"\012"+ "caption"+"\012"+ "text"+"\012";
    r+=Child::get(p,v);
  } else if (p=="caption"||p=="text")
    r=q2s(w->text());
  else if (p=="alignment") {
    if ((w->alignment())&Qt::AlignRight)
      r=std::string("right");
    else if ((w->alignment())&Qt::AlignHCenter)
      r=std::string("center");
    else
      r=std::string("left");
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Static::set(std::string p,std::string v)
{
  QLabel *w=(QLabel*) widget;
  QStringList opt=qsplit(v);
  if (p=="caption" || p=="text")
    w->setText(s2q(remquotes(v)));
  else if (p=="alignment") {
    if (opt.isEmpty()) {
      error("set alignment requires 1 argument: " + id + " " + p);
      return;
    }
    if (opt.at(0)=="left")
      w->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    else if (opt.at(0)=="right")
      w->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    else if (opt.at(0)=="center")
      w->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    else {
      error("set alignment requires left, right or center: " + id + " " + p);
      return;
    }
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Static::state()
{
  return "";
}
