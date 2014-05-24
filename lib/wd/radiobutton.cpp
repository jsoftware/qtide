
#include <QButtonGroup>
#include <QRadioButton>

#include "wd.h"
#include "checkbox.h"
#include "form.h"
#include "pane.h"
#include "radiobutton.h"

// ---------------------------------------------------------------------
RadioButton::RadioButton(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="radiobutton";
  QRadioButton *w=new QRadioButton;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  w->setText(qn);

  if (s=="group" && ppane->lasttype=="radiobutton") {
    if (!ppane->buttongroup) {
      Child *c=pform->children.last();
      ppane->buttongroup=new QButtonGroup;
      ppane->buttongroup->addButton((QRadioButton *)c->widget);
      c->grouped=true;
    }
    ppane->buttongroup->addButton(w);
    grouped=true;
  } else {
    grouped=false;
    ppane->buttongroup=(QButtonGroup *) 0;
  }

  connect(w,SIGNAL(toggled(bool)),
          this,SLOT(toggled(bool)));
}

// ---------------------------------------------------------------------
void RadioButton::set(string p,string v)
{
  QRadioButton *w=(QRadioButton*) widget;
  if (p=="caption" || p=="text")
    w->setText(s2q(v));
  else if (p=="value")
    w->setChecked(v=="1");
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string RadioButton::state()
{
  QRadioButton *w=(QRadioButton*) widget;
  return spair(id,w->isChecked()?(string)"1":(string)"0");
}

// ---------------------------------------------------------------------
void RadioButton::toggled(bool checked)
{
  if (grouped && checked==false) return;
  event="button";
  pform->signalevent(this);
}
