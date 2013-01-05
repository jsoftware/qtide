
#include <QLineEdit>

#include "wd.h"
#include "edit.h"
#include "form.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Edit::Edit(string s, string p, Form *f) : Child(s,p,f)
{
  type="edit";
  QLineEdit *w=new QLineEdit;
  widget=(QWidget*) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
  connect(w,SIGNAL(returnPressed()),
          this,SLOT(returnPressed()));
}

// ---------------------------------------------------------------------
void Edit::returnPressed()
{
  event="button";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Edit::set(string p)
{
  ((QLineEdit *)widget)->setText(s2q(p));
}

// ---------------------------------------------------------------------
string Edit::state()
{
  QLineEdit *w=(QLineEdit*) widget;
  int b,e;
  b=w->selectionStart();
  if (b<0)
    b=e=w->cursorPosition();
  else
    e=b+w->selectedText().size();
  string r;
  r+=spair(id,q2s(w->text()));
  r+=spair(id+"_select",i2s(b)+" "+i2s(e));
  return r;
}
