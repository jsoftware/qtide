
#include <QButtonGroup>
#include <QRadioButton>

#include "wd.h"
#include "checkbox.h"
#include "form.h"
#include "radiobutton.h"

// ---------------------------------------------------------------------
RadioButton::RadioButton(string s, string p, Form *f) : Child(s,p,f)
{
  type="radiobutton";
  QRadioButton *w=new QRadioButton;
  widget=(QWidget*) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
  w->setText(qs);

  if (p=="group" && form->lasttype=="radiobutton") {
    if (!form->buttongroup) {
      Child *c=form->children.last();
      form->buttongroup=new QButtonGroup;
      form->buttongroup->addButton((QRadioButton *)c->widget);
      c->grouped=true;
    }
    form->buttongroup->addButton(w);
    grouped=true;
  } else {
    grouped=false;
    form->buttongroup=(QButtonGroup *) 0;
  }

  connect(w,SIGNAL(toggled(bool)),
          this,SLOT(toggled(bool)));
}

// ---------------------------------------------------------------------
void RadioButton::set(string p)
{
  QRadioButton *w=(QRadioButton*) widget;
  w->setChecked(p=="1");
}

// ---------------------------------------------------------------------
void RadioButton::setp(string p,string v)
{
  if (p=="caption")
    ((QRadioButton *)widget)->setText(s2q(v));
  else Child::setp(p,v);
}

// ---------------------------------------------------------------------
string RadioButton::state()
{
  QRadioButton *w=(QRadioButton*) widget;
  return spair(id,w->isChecked()?"1":"0");
}

// ---------------------------------------------------------------------
void RadioButton::toggled(bool checked)
{
  if (signal==false) return;
  if (grouped && checked==false) return;
  event="button";
  pform->signalevent(this);
}
