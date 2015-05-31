
#include <QCheckBox>
#include <QStyle>

#include "wd.h"
#include "checkbox.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
CheckBox::CheckBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="checkbox";
  QCheckBox *w=new QCheckBox;
#ifdef QT_OS_ANDROID
  w->setStyleSheet(checkboxstyle(20*DM_density));
#endif
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);
  w->setText(qn);
  iconFile="";
  connect(w,SIGNAL(stateChanged(int)),
          this,SLOT(stateChanged()));
}

// ---------------------------------------------------------------------
string CheckBox::get(string p,string v)
{
  QCheckBox *w=(QCheckBox*) widget;
  string r;
  if (p=="property") {
    r+=string("caption")+"\012"+ "icon"+"\012"+ "text"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="caption"||p=="text")
    r=q2s(w->text());
  else if (p=="icon")
    r=iconFile;
  else if (p=="value")
    r=w->isChecked()?(string)"1":(string)"0";
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void CheckBox::set(string p,string v)
{
  QCheckBox *w=(QCheckBox*) widget;
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
  } else if (p=="value")
    w->setChecked(remquotes(v)!="0");
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string CheckBox::state()
{
  QCheckBox *w=(QCheckBox*) widget;
  return spair(id,w->isChecked()?(string)"1":(string)"0");
}

// ---------------------------------------------------------------------
void CheckBox::stateChanged()
{
  event="button";
  pform->signalevent(this);
}
