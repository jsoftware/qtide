
#include <QCheckBox>

#include "wd.h"
#include "checkbox.h"
#include "form.h"
#include "pane.h"

// ---------------------------------------------------------------------
CheckBox::CheckBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="checkbox";
  QCheckBox *w=new QCheckBox;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  w->setText(qn);
  connect(w,SIGNAL(stateChanged(int)),
          this,SLOT(stateChanged()));
}

// ---------------------------------------------------------------------
void CheckBox::set(string p,string v)
{
  QCheckBox *w=(QCheckBox*) widget;
  if (p=="caption" || p=="text")
    w->setText(s2q(v));
  else if (p=="value")
    w->setChecked(v=="1");
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string CheckBox::state()
{
  QCheckBox *w=(QCheckBox*) widget;
  return spair(id,w->isChecked()?"1":"0");
}

// ---------------------------------------------------------------------
void CheckBox::stateChanged()
{
  event="button";
  pform->signalevent(this);
}
