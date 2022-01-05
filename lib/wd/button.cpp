
#include <QPushButton>
#include <QSignalMapper>
#include <QStyle>

#include "button.h"
#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

// ---------------------------------------------------------------------
Button::Button(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="button";
  QPushButton *w=new QPushButton;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"default")) return;
  w->setObjectName(qn);
  childStyle(opt);
  w->setText(qn);
  iconFile="";
  if (opt.contains("default"))
    w->setDefault(true);
  connect(w,SIGNAL(clicked()), this,SLOT(clicked()));
}

// ---------------------------------------------------------------------
void Button::clicked()
{
  event="button";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
std::string Button::get(std::string p,std::string v)
{
  QPushButton *w=(QPushButton*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("caption")+"\012"+ "icon"+"\012"+ "text"+"\012";
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
void Button::set(std::string p,std::string v)
{
  QPushButton *w=(QPushButton*) widget;
  if (p=="caption" || p=="text")
    w->setText(s2q(remquotes(v)));
  else if (p=="icon") {
    QStringList qs=qsplit(v);
    QStringList sizes;
    if (!qs.size()) {
      error("missing parameters: " + p + " " + v);
      return;
    }
    if (qs.size()==2) {
      QString t=qs.at(1);
      if (qshasonly(t,"0123456789x")) {
        sizes=t.split('x');
        if (sizes.size()<2) {
          error("invalid icon width, height: " + p + " " + v);
          return;
        }
      } else {
        error("invalid icon width, height: " + p + " " + v);
        return;
      }
    }  else if (qs.size()>2) {
      error("extra parameters: " + p + " " + v);
      return;
    }
    iconFile=remquotes(q2s(qs.at(0)));
    int spi;
    if (iconFile.substr(0,8)=="qstyle::" && -1!=(spi=wdstandardicon(iconFile)))
      w->setIcon(w->style()->standardIcon((QStyle::StandardPixmap)spi));
    else
      w->setIcon(QIcon(s2q(iconFile)));
    if (qs.size()==2)
      w->setIconSize(QSize(c_strtoi(q2s(sizes.at(0))),c_strtoi(q2s(sizes.at(1)))));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Button::state()
{
  return "";
}
