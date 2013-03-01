
#include <QPlainTextEdit>
#include <QScrollBar>

#include "wd.h"
#include "editm.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Editm::Editm(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="editm";
  QPlainTextEdit *w=new QPlainTextEdit(p);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  w->setObjectName(qn);
  if (opt.contains("readonly"))
    w->setReadOnly(true);
}

// ---------------------------------------------------------------------
void Editm::set(string p)
{
  ((QPlainTextEdit*) widget)->setPlainText(s2q(p));
}

// ---------------------------------------------------------------------
void Editm::setp(string p,string v)
{
  QPlainTextEdit *w=(QPlainTextEdit*) widget;
  QStringList opt=qsplit(v);
  if (opt.isEmpty()) return;
  if (p=="limit")
    w->setMaximumBlockCount(c_strtoi(q2s(opt.at(0))));
  else Child::setp(p,v);
}

// ---------------------------------------------------------------------
string Editm::state()
{
  QPlainTextEdit *w=(QPlainTextEdit*) widget;
  QTextCursor c=w->textCursor();
  int b,e;
  b=c.selectionStart();
  e=c.selectionEnd();
  QScrollBar *h=w->horizontalScrollBar();
  QScrollBar *v=w->verticalScrollBar();
  string r;
  r+=spair(id,q2s(w->toPlainText()));
  r+=spair(id+"_select",i2s(b)+" "+i2s(e));
  r+=spair(id+"_scroll",i2s(h->value())+" "+i2s(v->value()));
  return r;
}
