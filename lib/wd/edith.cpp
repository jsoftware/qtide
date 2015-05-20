
#include <QTextEdit>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>

#include "../base/state.h"
#include "wd.h"
#include "edith.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#else
#include <QPrinter>
#include <QPrinterInfo>
#endif
#endif
// ---------------------------------------------------------------------
Edith::Edith(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="edith";
  QTextEdit *w=new QTextEdit;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  w->setReadOnly(true);
}

// ---------------------------------------------------------------------
void Edith::cmd(string p,string v)
{
  QTextEdit *w=(QTextEdit*) widget;
  QStringList opt=qsplit(v);
  if (p=="print") {
#ifndef QT_NO_PRINTER
    QTextDocument *d;
    d=w->document()->clone();
#ifdef QT50
    d->documentLayout()->setPaintDevice((QPagedPaintDevice *)config.Printer);
    d->setPageSize(QSizeF(config.Printer->pageRect().size()));
    d->print((QPagedPaintDevice *)config.Printer);
#else
    d->documentLayout()->setPaintDevice(config.Printer);
    d->setPageSize(QSizeF(config.Printer->pageRect().size()));
    d->print(config.Printer);
#endif
    delete d;
#else
    Q_UNUSED(w);
#endif
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
string Edith::get(string p,string v)
{
  QTextEdit *w=(QTextEdit*) widget;
  string r;
  if (p=="property") {
    r+=string("readonly")+"\012"+ "scroll"+"\012"+ "select"+"\012"+ "text"+"\012"+ "wrap"+"\012";
    r+=Child::get(p,v);
  } else if (p=="text")
    r=q2s(w->toHtml());
  else if (p=="select"||p=="scroll") {
    QTextCursor c=w->textCursor();
    int b,e;
    b=c.selectionStart();
    e=c.selectionEnd();
    QScrollBar *vb=w->verticalScrollBar();
    if (p=="select")
      r=i2s(b)+" "+i2s(e);
    else
      r=i2s(vb->value());
  } else if (p=="readonly")
    r=i2s(w->isReadOnly());
  else if (p=="wrap")
    r=i2s(w->lineWrapMode());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Edith::set(string p,string v)
{
  QTextEdit *w=(QTextEdit*) widget;
  QStringList opt=qsplit(v);
  QScrollBar *sb;

  int bgn,end,pos=0;

  if (p=="edit") {
    int s;
    if (opt.isEmpty())
      s=1;
    else
      s=c_strtoi(q2s(opt.at(0)));
    if (0==s) {
      if (!w->isReadOnly())  {
        QString t=w->toPlainText();
        w->setHtml(t);
        w->setReadOnly(1);
      }
    } else {
      if (w->isReadOnly()) {
        QString t=w->toHtml();
        w->setPlainText(t);
        w->setReadOnly(0);
      }
    }
  } else if (p=="text") {
    w->setHtml(s2q(remquotes(v)));
    w->setReadOnly(1);
  } else if (p=="select") {
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
      error("set scroll requires additional parameters: " + id + " " + p);
      return;
    }
  } else if (p=="wrap") {
    w->setLineWrapMode((remquotes(v)!="0")?QTextEdit::WidgetWidth:QTextEdit::NoWrap);
  } else if (p=="find") {
    w->find(opt.at(0));
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
