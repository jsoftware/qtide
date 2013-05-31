
#include <QTextEdit>
#include <QScrollBar>

#include "../base/state.h"
#include "wd.h"
#include "edith.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Edith::Edith(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="edith";
  QTextEdit *w=new QTextEdit(p);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  w->setObjectName(qn);
  if (opt.contains("readonly"))
    w->setReadOnly(true);
}

// ---------------------------------------------------------------------
void Edith::cmd(string p,string v)
{
  QTextEdit *w=(QTextEdit*) widget;
  QStringList opt=qsplit(v);
  if (p=="print") {
#ifndef ANDROID
    w->print(config.Printer);
#else
    Q_UNUSED(w);
#endif
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
void Edith::set(string p,string v)
{
  QTextEdit *w=(QTextEdit*) widget;
  QStringList opt=qsplit(v);
  QScrollBar *sb;

  int bgn,end,pos=0;

  if (p=="readonly")
    w->setReadOnly(remquotes(v)!="0");
  else if (p=="text")
    w->setHtml(s2q(v));
  else if (p=="select") {
    if (opt.isEmpty())
      w->selectAll();
    else {
      bgn=end=c_strtoi(q2s(opt.at(0)));
      if (opt.size()>1)
        end=c_strtoi(q2s(opt.at(1)));
      setselect(w,bgn,end);
    }
  } else if (p=="scroll") {
    if (opt.size()) {
      sb=w->verticalScrollBar();
      if (opt.at(0)=="min")
        pos=sb->minimum();
      else if (opt.at(0)=="max")
        pos=sb->maximum();
      else
        pos=c_strtoi(q2s(opt.at(0)));
      sb->setValue(pos);
    } else {
      error("set scroll requires additional parameters: " + p);
      return;
    }
  } else if (p=="wrap") {
    w->setLineWrapMode((remquotes(v)!="0")?QTextEdit::WidgetWidth:QTextEdit::NoWrap);
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
void Edith::setselect(QTextEdit *w, int bgn, int end)
{
  QTextCursor c = w->textCursor();
  c.setPosition(end,QTextCursor::MoveAnchor);
  c.setPosition(bgn,QTextCursor::KeepAnchor);
  w->setTextCursor(c);
}

// ---------------------------------------------------------------------
string Edith::state()
{
  QTextEdit *w=(QTextEdit*) widget;
  QTextCursor c=w->textCursor();
  int b,e;
  b=c.selectionStart();
  e=c.selectionEnd();
  QScrollBar *v=w->verticalScrollBar();
  string r;
  r+=spair(id,q2s(w->toHtml()));
  r+=spair(id+"_select",i2s(b)+" "+i2s(e));
  r+=spair(id+"_scroll",i2s(v->value()));
  return r;
}
