
#include <QPainter>
#include <QPaintEngine>
#include <math.h>

#include "opengl.h"
#include "opengl2.h"
#include "form.h"
#include "pane.h"

extern Opengl *opengl;

// ---------------------------------------------------------------------
Opengl2::Opengl2(Child *c)
{
  pchild = c;
  active=false;
  nopaint=false;
  jpaint=false;
  epaint=false;
  initialized = false;
  pixmap=0;
  painter=0;
  fontheight=0;
  setAttribute(Qt::WA_DeleteOnClose);
//  this->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
//  this->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
  updateGeometry();
  setMouseTracking (true);         // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event

  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_NoSystemBackground);
  setAutoBufferSwap (false);
}

// ---------------------------------------------------------------------
void Opengl2::initializeGL()
{
//  qDebug() << "opengl2 initializeGL";

  pchild->event="initialize";
  pchild->pform->signalevent(pchild);

//  qDebug() << "opengl2 initializeGL exit";
  initialized = true;

}


// ---------------------------------------------------------------------
void Opengl2::paintgl()
{
  this->paintGL();
}

// ---------------------------------------------------------------------
void Opengl2::paintGL()
{
  if (!initialized) return;

//  qDebug() << "opengl2 paintGL";

  QPainter painter;
  painter.begin(this);
  painter.beginNativePainting();

  epaint=true;
  if (!nopaint) {
    jpaint=true;
    pchild->event="paint";
    pchild->pform->signalevent(pchild);
    jpaint=false;
  }

  painter.endNativePainting();

  if (!nopaint) {
    this->active=true;
    this->painter=&painter;
    jpaint=true;
    pchild->event="paintz";
    pchild->pform->signalevent(pchild);
    jpaint=false;
    this->painter=0;
    this->active=false;
  }

  painter.end();
  swapBuffers();
  epaint=false;

//  qDebug() << "opengl2 paintGL exit";
}

// ---------------------------------------------------------------------
void Opengl2::resizeGL ( int width, int height )
{
//  qDebug() << "opengl2 resizeGL";
//  qDebug() << "opengl2 resizeGL new size " + QString::number(width) + " " + QString::number(height);

  Q_UNUSED(width);
  Q_UNUSED(height);

  pchild->event="resize";
  pchild->pform->signalevent(pchild);

//  qDebug() << "opengl2 resizeGL exit";
}

// ---------------------------------------------------------------------
void Opengl2::buttonEvent (QEvent::Type type, QMouseEvent *event)
{
  opengl=(Opengl *)pchild;

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
  sprintf (sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[200];
  sprintf (sysdata , "%d %d %d %d %d %d %d %d %d %d %d %d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);

  pchild->event=evtname;
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::wheelEvent (QWheelEvent *event)
{
  opengl=(Opengl *)pchild;

  char deltasign = ' ';
  int delta = event->delta() / 8;  // degree
  if (delta<0) {
    delta = -delta;
    deltasign = '_';
  }

  // sysmodifiers = shift+2*control
  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysmodifiers[20];
  sprintf (sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[200];
  sprintf (sysdata , "%d %d %d %d %d %d %d %d %d %d %d %c%d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, deltasign, delta);

  pchild->event=string("mwheel");
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::mousePressEvent(QMouseEvent *event)
{
  buttonEvent (QEvent::MouseButtonPress, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseMoveEvent(QMouseEvent *event)
{
  buttonEvent (QEvent::MouseMove, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent (QEvent::MouseButtonDblClick, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseReleaseEvent(QMouseEvent *event)
{
  buttonEvent (QEvent::MouseButtonRelease, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseWheelEvent(QWheelEvent *event)
{
  wheelEvent (event);
}

// ---------------------------------------------------------------------
void Opengl2::focusInEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focus";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::focusOutEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focuslost";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::keyPressEvent(QKeyEvent *event)
{
  // sysmodifiers = shift+2*control
  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,0
  char sysmodifiers[20];
  sprintf (sysmodifiers , "%d", (2*(!!(event->modifiers() & Qt::CTRL))) + (!!(event->modifiers() & Qt::SHIFT)));
  char sysdata[20];
  QString keyt = event->text();
  if (keyt.size())
    sprintf (sysdata , "%s", keyt.toUtf8().constData());
  else sprintf (sysdata , "%s", QString(QChar(event->key())).toUtf8().constData());

  pchild->event=string("char");
  pchild->sysmodifiers=string(sysmodifiers);
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
  QWidget::keyPressEvent(event);
}

// ---------------------------------------------------------------------
Opengl2::~Opengl2()
{
//  qDebug() << "opengl2 deleted";

  if (pchild==(Child *)opengl) {
    opengl=0;
//    qDebug() << "opengl=0";
  }
}

