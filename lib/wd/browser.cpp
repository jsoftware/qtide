
#include <QDir>
#include <QAbstractTextDocumentLayout>

#include "browser.h"
#include "cmd.h"
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
Browser::Browser(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="browser";
  Qbrowser *w=new Qbrowser(this,p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  baseUrl = QUrl();
  connect(w,SIGNAL(sourceChanged(const QUrl &)), this,SLOT(sourceChanged( const QUrl &)));
}

// ---------------------------------------------------------------------
void Browser::cmd(std::string p,std::string v)
{
  Qbrowser *w = (Qbrowser *)widget;
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
  } else if (p=="backward") {
    w->backward();
  } else if (p=="forward") {
    w->forward();
  } else if (p=="reload") {
    w->reload();
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Browser::get(std::string p,std::string v)
{
  Qbrowser *w = (Qbrowser *)widget;
  std::string r;
  if (p=="property") {
    r+=std::string("baseurl")+"\012"+ "html"+"\012"+ "openexternallinks"+"\012"+ "openlinks"+"\012"+ "source"+"\012";
    r+=Child::get(p,v);
  } else if (p=="baseurl")
    r=q2s(baseUrl.toString());
  else if (p=="html")
    r=q2s(w->toHtml());
  else if (p=="openexternallinks")
    r=i2s(w->openExternalLinks());
  else if (p=="openlinks")
    r=i2s(w->openLinks());
  else if (p=="source")
    r=q2s(w->source().toString());
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Browser::set(std::string p,std::string v)
{
  Qbrowser *w = (Qbrowser *)widget;
  if (p=="baseurl") {
    QString t = s2q(remquotes(v));
    baseUrl = QUrl(t);
    w->document()->setMetaInformation(QTextDocument::DocumentUrl, QUrl::fromLocalFile(baseUrl.toString()).toString());
  } else if (p=="html") {
    w->setHtml(s2q(remquotes(v)));
  } else if (p=="openexternallinks") {
    w->setOpenExternalLinks(remquotes(v)!="0");
  } else if (p=="openlinks") {
    w->setOpenLinks(remquotes(v)!="0");
  } else if (p=="source") {
    QString t = s2q(remquotes(v));
    if(isroot(t) && !t.startsWith("file:"))
      w->setSource(QUrl::fromLocalFile(t));
    else
      w->setSource(w->source().resolved(QUrl(t)));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Browser::state()
{
  Qbrowser *w = (Qbrowser *)widget;
  Q_UNUSED(w);
  std::string r;
  if (event=="mmove") return r;
  if (event=="source") {
    r+=spair(id+"_baseurl",q2s(baseUrl.toString()));
  }
  return r;
}

// ---------------------------------------------------------------------
void Browser::sourceChanged(const QUrl & source)
{
  Qbrowser *w = (Qbrowser *)widget;
  baseUrl = baseUrl.resolved(source.toString());
  QString t = baseUrl.toString();
  if(isroot(t) && !t.startsWith("file:"))
    w->document()->setMetaInformation(QTextDocument::DocumentUrl, QUrl::fromLocalFile(t).toString());
  else
    w->document()->setMetaInformation(QTextDocument::DocumentUrl, t);
// not sure why this line is needed
//  w->setSearchPaths(QStringList(cfpath(baseUrl.toString())));
  event="source";
  pform->signalevent(this);
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void Browser::printPreview(QPrinter * printer)
{
  Qbrowser *w = (Qbrowser *)widget;
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

// ---------------------------------------------------------------------
Qbrowser::Qbrowser(Child *c, QWidget *parent)
{
  Q_UNUSED(parent);
  pchild=c;
  setMouseTracking(true);           // for mmove event
  setOpenExternalLinks(true);
}

// ---------------------------------------------------------------------
void Qbrowser::buttonEvent(QEvent::Type type, QMouseEvent *event)
{
  std::string lmr = "";
  switch (event->button()) {
  case Qt::LeftButton:
    lmr = "l";
    break;
  case Qt::MiddleButton:
    lmr = "m";
    break;
  case Qt::RightButton:
    lmr = "r";
    break;
  default:
    break;
  }

  std::string evtname = "mmove";
  switch (type) {
  case QEvent::MouseButtonPress:
    evtname = "mb" + lmr + "down";
    break;
  case QEvent::MouseButtonRelease:
    evtname = "mb" + lmr + "up";
    break;
  case QEvent::MouseButtonDblClick:
    evtname = "mb" + lmr + "dbl";
    break;
  case QEvent::MouseMove:
    evtname = "mmove";
    break;
  default:
    break;
  }

  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysdata[200];
  sprintf(sysdata, "%d %d %d %d %d %d %d %d %d %d %d %d",
#if defined(QT60)
          (int)event->position().x(), (int)event->position().y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
#else
          event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
#endif
          (!!(event->buttons() & Qt::MiddleButton)), (!!(event->modifiers() & Qt::CTRL)),
          (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);

  pchild->event=evtname;
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=std::string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Qbrowser::wheelEvent(QWheelEvent *event)
{
  char deltasign = ' ';
#if defined(QT50)
  int delta = event->angleDelta().y() / 8;  // degree
#else
  int delta = event->delta() / 8;  // degree
#endif
  if (delta<0) {
    delta = -delta;
    deltasign = '_';
  }

  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysdata[200];
  sprintf(sysdata, "%d %d %d %d %d %d %d %d %d %d %d %c%d",
#if defined(QT515)
          (int)event->position().x(), (int)event->position().y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
#else
          event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
#endif
          (!!(event->buttons() & Qt::MiddleButton)), (!!(event->modifiers() & Qt::CTRL)),
          (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, deltasign, delta);

  pchild->event=std::string("mwheel");
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=std::string(sysdata);
  pchild->pform->signalevent(pchild);
  QTextBrowser::wheelEvent(event);
}

// ---------------------------------------------------------------------
void Qbrowser::mousePressEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonPress, event);
  QTextBrowser::mousePressEvent(event);
}

// ---------------------------------------------------------------------
void Qbrowser::mouseMoveEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseMove, event);
  QTextBrowser::mouseMoveEvent(event);
}

// ---------------------------------------------------------------------
void Qbrowser::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonDblClick, event);
  QTextBrowser::mouseDoubleClickEvent(event);
}

// ---------------------------------------------------------------------
void Qbrowser::mouseReleaseEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonRelease, event);
  QTextBrowser::mouseReleaseEvent(event);
}

