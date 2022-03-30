
#include <QTextEdit>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include <QUrl>

#include "cmd.h"
#include "edith.h"
#include "form.h"
#include "pane.h"
#include "wd.h"
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
Edith::Edith(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="edith";
  QTextEdit *w=new QTextEdit;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);
  w->setReadOnly(true);
}

// ---------------------------------------------------------------------
void Edith::cmd(std::string p,std::string v)
{
  QStringList opt=qsplit(v);
  if (p=="print") {
#ifndef QT_NO_PRINTER
    printPreview(config.Printer);
#endif
  } else if (p=="printpreview") {
#ifndef QT_NO_PRINTER
    QPrintPreviewDialog *dlg = new QPrintPreviewDialog(config.Printer, pform);
    dlg->setWindowTitle("Preview Document");
    QObject::connect(dlg,SIGNAL(paintRequested(QPrinter *)),this,SLOT(printPreview(QPrinter *)));
    dlg->exec();
    delete dlg;
    config.Printer->setPrintRange(QPrinter::AllPages);
#endif
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Edith::get(std::string p,std::string v)
{
  QTextEdit *w=(QTextEdit*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("readonly")+"\012"+ "scroll"+"\012"+ "select"+"\012"+ "text"+"\012"+ "wrap"+"\012";
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
void Edith::set(std::string p,std::string v)
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
  } else if (p=="baseurl") {
    QString t = s2q(remquotes(v));
    QUrl baseUrl = QUrl(t);
    if(isroot(t) && !t.startsWith("file:"))
      w->document()->setMetaInformation(QTextDocument::DocumentUrl, QUrl::fromLocalFile(t).toString());
    else
      w->document()->setMetaInformation(QTextDocument::DocumentUrl, t);
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
std::string Edith::state()
{
  QTextEdit *w=(QTextEdit*) widget;
  QTextCursor c=w->textCursor();
  int b,e;
  b=c.selectionStart();
  e=c.selectionEnd();
  QScrollBar *v=w->verticalScrollBar();
  std::string r;
  r+=spair(id,q2s(w->toHtml()));
  r+=spair(id+"_select",i2s(b)+" "+i2s(e));
  r+=spair(id+"_scroll",i2s(v->value()));
  return r;
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void Edith::printPreview(QPrinter * printer)
{
  QTextEdit *w=(QTextEdit*) widget;
  QTextDocument *d=w->document()->clone();
#ifdef QT50
  d->documentLayout()->setPaintDevice((QPagedPaintDevice *)printer);
#ifdef QT62
  d->setPageSize(printer->pageLayout().paintRectPixels(printer->resolution()).size());
#else
  d->setPageSize(QSizeF(printer->pageRect().size()));
#endif
  d->print((QPagedPaintDevice *)printer);
#else
  d->documentLayout()->setPaintDevice(printer);
  d->setPageSize(QSizeF(printer->pageRect().size()));
  d->print(printer);
#endif
  delete d;
}
#endif
