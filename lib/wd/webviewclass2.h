
#if defined(WEBKITVIEW)

#ifdef QT50
#include <QtWebKitWidgets>
#else
#include <QtWebKit>
#endif
#include "webkitview.h"

#define QTWEBVIEW QWebView
#define QWEBVIEW Webkitview
#define WEBVIEW Webview

#elif defined(WEBENGINEVIEW)

#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "webengineview.h"

#define QTWEBVIEW QWebEngineView
#define QWEBVIEW Webengineview
#define WEBVIEW Webview2

#endif

#include "form.h"

// ---------------------------------------------------------------------
QWEBVIEW::QWEBVIEW(Child *c, QWidget *parent) : QTWEBVIEW()
{
  Q_UNUSED(parent);
  pchild = c;
  setContentsMargins(0,0,0,0);
  setAttribute(Qt::WA_DeleteOnClose);
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setMouseTracking(true);           // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event
}

// ---------------------------------------------------------------------
void QWEBVIEW::buttonEvent(QEvent::Type type, QMouseEvent *event)
{
  string lmr = "";
  switch (event->button()) {
  case Qt::LeftButton:
    lmr = "l";
    break;
  case Qt::MidButton:
    lmr = "m";
    break;
  case Qt::RightButton:
    lmr = "r";
    break;
  default:
    break;
  }

  string evtname = "mmove";
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

  // sysmodifiers = shift+2*control
  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysmodifiers[20];
  sprintf(sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[200];
  sprintf(sysdata , "%d %d %d %d %d %d %d %d %d %d %d %d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);

  pchild->event=evtname;
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void QWEBVIEW::wheelEvent(QWheelEvent *event)
{
  char deltasign = ' ';
  int delta = event->delta() / 8;  // degree
  if (delta<0) {
    delta = -delta;
    deltasign = '_';
  }

  // sysmodifiers = shift+2*control
  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysmodifiers[20];
  sprintf(sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[200];
  sprintf(sysdata , "%d %d %d %d %d %d %d %d %d %d %d %c%d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, deltasign, delta);

  pchild->event=string("mwheel");
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
  QTWEBVIEW::wheelEvent(event);
}

// ---------------------------------------------------------------------
void QWEBVIEW::mousePressEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonPress, event);
  QTWEBVIEW::mousePressEvent(event);
}

// ---------------------------------------------------------------------
void QWEBVIEW::mouseMoveEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseMove, event);
  QTWEBVIEW::mouseMoveEvent(event);
}

// ---------------------------------------------------------------------
void QWEBVIEW::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonDblClick, event);
  QTWEBVIEW::mouseDoubleClickEvent(event);
}

// ---------------------------------------------------------------------
void QWEBVIEW::mouseReleaseEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonRelease, event);
  QTWEBVIEW::mouseReleaseEvent(event);
}

// ---------------------------------------------------------------------
void QWEBVIEW::focusInEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focus";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
  QTWEBVIEW::focusInEvent(event);
}

// ---------------------------------------------------------------------
void QWEBVIEW::focusOutEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focuslost";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
  QTWEBVIEW::focusOutEvent(event);
}

// ---------------------------------------------------------------------
void QWEBVIEW::keyPressEvent(QKeyEvent *event)
{
  // sysmodifiers = shift+2*control
  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,0
  int key1=0;
  int key=event->key();
#ifdef QT_OS_ANDROID
  if (key==Qt::Key_Back) {
    QWidget::keyPressEvent(event);
    return;
  }
#endif
  if ((key>0x10000ff)||((key>=Qt::Key_F1)&&(key<=Qt::Key_F35))) {
    QWidget::keyPressEvent(event);
    return;
  } else if (key>=0x1000000) {
    key1=(key & 0xff) | 0xf800;
  }
  char sysmodifiers[20];
  sprintf(sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[20];
  if (!key1)
    sprintf(sysdata , "%s", event->text().toUtf8().constData());
  else sprintf(sysdata , "%s", QString(QChar(key1)).toUtf8().constData());

  pchild->event=string("char");
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
  QTWEBVIEW::keyPressEvent(event);
}
