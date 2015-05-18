
#include <QScrollBar>

#include "../base/plaintextedit.h"
#include "../base/state.h"
#include "wd.h"
#include "editm.h"
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
Editm::Editm(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="editm";
  EditmPTE *w=new EditmPTE;
  w->pchild=this;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("readonly selectable")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  if (opt.contains("readonly")) {
    w->setReadOnly(true);
    if (opt.contains("selectable"))
      w->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
  }
}

// ---------------------------------------------------------------------
void Editm::cmd(string p,string v)
{
  EditmPTE *w=(EditmPTE*) widget;
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
string Editm::get(string p,string v)
{
  EditmPTE *w=(EditmPTE*) widget;
  string r;
  if (p=="property") {
    r+=string("limit")+"\012"+ "readonly"+"\012"+ "scroll"+"\012"+ "select"+"\012"+ "text"+"\012"+ "wrap"+"\012";
    r+=Child::get(p,v);
  } else if (p=="text")
    r=q2s(w->toPlainText());
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
  } else if (p=="limit")
    r=i2s(w->maximumBlockCount());
  else if (p=="readonly")
    r=i2s(w->isReadOnly());
  else if (p=="wrap")
    r=i2s(w->lineWrapMode());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Editm::set(string p,string v)
{
  EditmPTE *w=(EditmPTE*) widget;
  string r;
  QStringList opt=qsplit(v);
  QScrollBar *sb;

  int bgn,end,pos=0;

  if (p=="limit") {
    if (opt.isEmpty()) {
      error("set limit requires 1 number: " + id + " " + p);
      return;
    }
    w->setMaximumBlockCount(c_strtoi(q2s(opt.at(0))));
  } else if (p=="readonly")
    w->setReadOnly(remquotes(v)!="0");
  else if (p=="text")
    w->setPlainText(s2q(remquotes(v)));
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
      error("set scroll requires additional parameters: " + id + " " + p);
      return;
    }
  } else if (p=="wrap") {
    w->setLineWrapMode((remquotes(v)!="0")?PlainTextEdit::WidgetWidth:PlainTextEdit::NoWrap);
  } else if (p=="find") {
    w->find(opt.at(0));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
void Editm::setselect(PlainTextEdit *w, int bgn, int end)
{
  QTextCursor c = w->textCursor();
  c.setPosition(end,QTextCursor::MoveAnchor);
  c.setPosition(bgn,QTextCursor::KeepAnchor);
  w->setTextCursor(c);
}

// ---------------------------------------------------------------------
string Editm::state()
{
  EditmPTE *w=(EditmPTE*) widget;
  QTextCursor c=w->textCursor();
  int b,e;
  b=c.selectionStart();
  e=c.selectionEnd();
  QScrollBar *v=w->verticalScrollBar();
  string r;
  r+=spair(id,q2s(w->toPlainText()));
  r+=spair(id+"_select",i2s(b)+" "+i2s(e));
  r+=spair(id+"_scroll",i2s(v->value()));
  return r;
}

// ---------------------------------------------------------------------
EditmPTE::EditmPTE(QWidget *parent) : PlainTextEdit(parent) {}

// ---------------------------------------------------------------------
void EditmPTE::keyPressEvent(QKeyEvent *event)
{
  if ((event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return) && !(event->modifiers() & Qt::CTRL) && !(event->modifiers() & Qt::SHIFT)) {
    if (isReadOnly()) {
      char sysmodifiers[20];
      sprintf(sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
      pchild->event=string("button");
      pchild->sysmodifiers=string(sysmodifiers);
      pchild->pform->signalevent(pchild);
    }
    PlainTextEdit::keyPressEvent(event);
    return;
  }
  int key1=0;
  int key=event->key();
  if ((key>0x10000ff)||((key>=Qt::Key_F1)&&(key<=Qt::Key_F35))) {
    PlainTextEdit::keyPressEvent(event);
    return;
  } else
    key1=translateqkey(key);
  char sysmodifiers[20];
  sprintf(sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[20];
  if (key==key1)
    sprintf(sysdata , "%s", event->text().toUtf8().constData());
  else sprintf(sysdata , "%s", QString(QChar(key1)).toUtf8().constData());

  pchild->event=string("char");
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
  PlainTextEdit::keyPressEvent(event);
}
