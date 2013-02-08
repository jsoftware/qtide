
#include <QLineEdit>

#include "wd.h"
#include "edit.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Edit::Edit(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="edit";
  QLineEdit *w=new QLineEdit;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
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
