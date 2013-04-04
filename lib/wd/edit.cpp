
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
  QLineEdit *w=new QLineEdit(p);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  w->setObjectName(qn);

  if (opt.contains("password"))
    w->setEchoMode(QLineEdit::Password);

  if (opt.contains("readonly"))
    w->setReadOnly(true);

  if (opt.contains("left"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  else if (opt.contains("right"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  else if (opt.contains("center"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

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
void Edit::set(string p,string v)
{
  QLineEdit *w = (QLineEdit *)widget;

  if (p=="text") {
    w->setText(s2q(v));
    return;
  } else if (p=="limit") {
    QStringList opt=qsplit(v);
    if (opt.isEmpty()) {
      error("set limit requires 1 number: " + p);
      return;
    }
    w->setMaxLength(c_strtoi(q2s(opt.at(0))));
  } else Child::set(p,v);
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
