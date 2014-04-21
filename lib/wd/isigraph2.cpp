#include <QTimer>
#include <QPainter>
#include <QtGui/qmatrix4x4.h>

#include "isigraph.h"
#include "isigraph2.h"
#include "form.h"
#include "pane.h"

extern "C" Dllexport int glclear2(void *);
extern Child *isigraph;

// ---------------------------------------------------------------------
Isigraph2::Isigraph2(Child *c, QWidget *parent) : QWidget()
{
  Q_UNUSED(parent);
  pchild = c;
  orgx=0;
  orgy=0;
  painter=0;
  pixmap=0;
  fontheight=0;
  setContentsMargins(0,0,0,0);
  setAttribute(Qt::WA_DeleteOnClose);
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setMouseTracking(true);           // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event
}

// ---------------------------------------------------------------------
void Isigraph2::fill(const int *p)
{
  QColor c(*(p), *(p + 1), *(p + 2));
  if (pixmap)
    pixmap->fill(c);
  else
    painter->fillRect(0,0,width(),height(),c);
}

// ---------------------------------------------------------------------
QPixmap Isigraph2::getpixmap()
{
  if (pixmap)
    return pixmap->copy(0,0,width(),height());
  return QPixmap::grabWidget(this, 0, 0, width(), height());
}

// ---------------------------------------------------------------------
void Isigraph2::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  if (type=="isidraw")
    paintEvent_isidraw();
  else
    paintEvent_isigraph();
}

// ---------------------------------------------------------------------
void Isigraph2::paintEvent_isidraw()
{
  if (!pixmap) return;
  QPainter p(this);
  p.drawPixmap(0,0,*pixmap,0,0,width(),height());
}

// ---------------------------------------------------------------------
void Isigraph2::paintEvent_isigraph()
{
  if (painter) return;
  painter=new QPainter(this);
  painter->setRenderHint(QPainter::Antialiasing, true);
  pchild->event="paint";
  pchild->pform->signalevent(pchild);
  paintend();
}

// ---------------------------------------------------------------------
void Isigraph2::paintend()
{
  if (painter) {
    painter->end();
    delete painter;
    painter=0;
  }
}

// ---------------------------------------------------------------------
void Isigraph2::resizeEvent(QResizeEvent *event)
{
  if (type=="isigraph") return;
  QSize s=event->size();
  int w=s.width();
  int h=s.height();
  if (pixmap) {
    if (w > pixmap->width() || h > pixmap->height()) {
      if (painter) delete painter;
      QPixmap *p=new QPixmap(w+128,h+128);
      painter=new QPainter(p);
      painter->drawPixmap(QPoint(0,0),*pixmap);
      delete pixmap;
      pixmap=p;
    }
  } else {
    pixmap=new QPixmap(w,h);
    painter=new QPainter(pixmap);
  }
  painter->setRenderHint(QPainter::Antialiasing, true);
  pchild->event="resize";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Isigraph2::buttonEvent(QEvent::Type type, QMouseEvent *event)
{
  //if (!active) return;
  isigraph=(Isigraph *)pchild;

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
void Isigraph2::wheelEvent(QWheelEvent *event)
{
  isigraph=(Isigraph *)pchild;

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
}

// ---------------------------------------------------------------------
void Isigraph2::mousePressEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonPress, event);
}

// ---------------------------------------------------------------------
void Isigraph2::mouseMoveEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseMove, event);
}

// ---------------------------------------------------------------------
void Isigraph2::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonDblClick, event);
}

// ---------------------------------------------------------------------
void Isigraph2::mouseReleaseEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonRelease, event);
}

// ---------------------------------------------------------------------
void Isigraph2::mouseWheelEvent(QWheelEvent *event)
{
  wheelEvent(event);
}

// ---------------------------------------------------------------------
void Isigraph2::focusInEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focus";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Isigraph2::focusOutEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focuslost";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Isigraph2::keyPressEvent(QKeyEvent *event)
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
  QWidget::keyPressEvent(event);
}

// ---------------------------------------------------------------------
Isigraph2::~Isigraph2()
{
  if (pchild==(Child *)isigraph) {
    isigraph=0;
    if (painter) delete painter;
    if (pixmap) delete pixmap;
  }
}
