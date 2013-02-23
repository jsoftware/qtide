
#include <QPlainTextEdit>
#include <QScrollBar>

#include "wd.h"
#include "editm.h"
#include "form.h"
#include "pane.h"

// ---------------------------------------------------------------------
Editm::Editm(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="editm";
  QPlainTextEdit *w=new QPlainTextEdit(p);
  widget=(QWidget*) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
}

// ---------------------------------------------------------------------
void Editm::set(string p)
{
  ((QPlainTextEdit*) widget)->setPlainText(s2q(p));
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
