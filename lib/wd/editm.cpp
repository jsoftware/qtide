
#include <QScrollBar>

#include "cmd.h"
#include "editm.h"
#include "form.h"
#include "pane.h"
#include "wd.h"
#include "../base/dialog.h"
#include "../base/plaintextedit.h"
#include "../base/state.h"
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#include <QtPrintSupport/QPrintPreviewDialog>
#else
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintPreviewDialog>
#endif
#endif

// ---------------------------------------------------------------------
Editm::Editm(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="editm";
  EditmPTE *w=new EditmPTE;
  w->pchild=this;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"readonly selectable")) return;
  w->setObjectName(qn);
  childStyle(opt);
  if (opt.contains("readonly")) {
    w->setReadOnly(true);
    if (opt.contains("selectable"))
      w->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
  }
}

// ---------------------------------------------------------------------
void Editm::cmd(std::string p,std::string v)
{
  QStringList opt=qsplit(v);
  if (p=="print") {
#ifndef QT_NO_PRINTER
    ((EditmPTE*) widget)->printPreview(config.Printer);
#endif
  } else if (p=="printpreview") {
#ifndef QT_NO_PRINTER
    QPrintPreviewDialog *dlg = new QPrintPreviewDialog(config.Printer, pform);
    dlg->setWindowTitle("Preview Document");
    QObject::connect(dlg,SIGNAL(paintRequested(QPrinter *)),((EditmPTE*) widget),SLOT(printPreview(QPrinter *)));
    dlg->exec();
    delete dlg;
    config.Printer->setPrintRange(QPrinter::AllPages);
#endif
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Editm::get(std::string p,std::string v)
{
  EditmPTE *w=(EditmPTE*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("limit")+"\012"+ "readonly"+"\012"+ "scroll"+"\012"+ "select"+"\012"+ "text"+"\012"+ "wrap"+"\012";
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
void Editm::set(std::string p,std::string v)
{
  EditmPTE *w=(EditmPTE*) widget;
  std::string r;
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
std::string Editm::state()
{
  EditmPTE *w=(EditmPTE*) widget;
  QTextCursor c=w->textCursor();
  int b,e;
  b=c.selectionStart();
  e=c.selectionEnd();
  QScrollBar *v=w->verticalScrollBar();
  std::string r;
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
  int key=event->key();
  if (ismodifier(key)) return;
  if ((key==Qt::Key_Enter || key==Qt::Key_Return) && !(event->modifiers() & Qt::CTRL) && !(event->modifiers() & Qt::SHIFT)) {
    if (isReadOnly()) {
      pchild->event=std::string("button");
      pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
      pchild->pform->signalevent(pchild);
      return;
      // note we don't fall through to handle keyPressEvent in the widget.
      // it shouldn't do anything but move the cursor, and we have already jabbed the
      // _button event, so don't let the widget override something the button calls for
    }
  }
  // _char events not given from editm now, so just pass the event on
  PlainTextEdit::keyPressEvent(event);
}
