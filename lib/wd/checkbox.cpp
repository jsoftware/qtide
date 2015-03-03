
#include <QCheckBox>

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
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  w->setText(qn);
  connect(w,SIGNAL(stateChanged(int)),
          this,SLOT(stateChanged()));
}

// ---------------------------------------------------------------------
void CheckBox::set(string p,string v)
{
  QCheckBox *w=(QCheckBox*) widget;
  if (p=="caption" || p=="text")
    w->setText(s2q(remquotes(v)));
  else if (p=="value")
    w->setChecked(v=="1");
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
