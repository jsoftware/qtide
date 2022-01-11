
#include <QPainter>

#include "form.h"
#include "opengl2.h"
#include "opengl.h"
#include "pane.h"

extern Child *opengl;
extern "C" int gl_clear2 (void *p,int clear);

// ---------------------------------------------------------------------
#ifdef USE_QOpenGLWidget
Opengl2::Opengl2(Child *c, const QSurfaceFormat& format, QWidget *parent) : QOpenGLWidget(parent)
#else
Opengl2::Opengl2(Child *c, const QGLFormat& format, QWidget *parent) : QGLWidget(format,parent)
#endif
{
  Q_UNUSED(format);
  Q_UNUSED(parent);
#ifdef USE_QOpenGLWidget
  this->setFormat(format);
#endif
  pchild = c;
  initialized = false;
  painter=0;
  font=0;
  gl_clear2(this,0);
  setMouseTracking(true);           // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event
}

// ---------------------------------------------------------------------
Opengl2::~Opengl2()
{
  if (pchild==opengl)
    opengl=0;
  if (painter) {
    painter->end();
    delete painter;
    painter=0;
  }
}

#ifdef USE_QOpenGLWidget
// ---------------------------------------------------------------------
void Opengl2::updateGL()
{
  this->update();
}
#endif

// ---------------------------------------------------------------------
void Opengl2::fill(const int *p)
{
  QColor c(*(p), *(p + 1), *(p + 2), *(p + 3));
  if (painter)
    painter->fillRect(0,0,width(),height(),c);
}

// ---------------------------------------------------------------------
QPixmap Opengl2::getpixmap()
{
  QPixmap m;
  if (painter) return m;
  QPixmap p(size());
  render(&p);
  return p;
}

// ---------------------------------------------------------------------
void Opengl2::initializeGL()
{
  pchild->event="initialize";
  pchild->pform->signalevent(pchild);
  initialized = true;
}

// ---------------------------------------------------------------------
void Opengl2::paintGL()
{
  if (!initialized) return;
  painter=new QPainter(this);
  pchild->event="paint";
  pchild->pform->signalevent(pchild);
  paintend();
}

// ---------------------------------------------------------------------
void Opengl2::paintend()
{
  if (painter) {
    painter->end();
    delete painter;
    painter=0;
  }
}

// ---------------------------------------------------------------------
void Opengl2::resizeGL(int width, int height)
{
  Q_UNUSED(width);
  Q_UNUSED(height);

  pchild->event="resize";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::buttonEvent(QEvent::Type type, QMouseEvent *event)
{
  opengl=pchild;

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
#if defined(QT60)
  sprintf(sysdata, "%d %d %d %d %d %d %d %d %d %d %d %d", (int)event->position().x(), (int)event->position().y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MiddleButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);
#else
  sprintf(sysdata, "%d %d %d %d %d %d %d %d %d %d %d %d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MiddleButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);
#endif

  pchild->event=evtname;
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=std::string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::wheelEvent(QWheelEvent *event)
{
  opengl=pchild;

  char deltasign = ' ';
#ifdef QT50
  int delta = event->angleDelta().y() / 8;  // degree
#else
  int delta = event->delta() / 8;  // degree
#endif
  if (delta<0) {
    delta = -delta;
    deltasign = '_';
  }

  Q_UNUSED(deltasign);

  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysdata[200];
#if defined(QT515)
  sprintf(sysdata, "%d %d %d %d %d %d %d %d %d %d %d %d", (int)event->position().x(), (int)event->position().y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MiddleButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);
#else
  sprintf(sysdata, "%d %d %d %d %d %d %d %d %d %d %d %c%d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MiddleButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, deltasign, delta);
#endif

  pchild->event=std::string("mwheel");
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=std::string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Opengl2::mousePressEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonPress, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseMoveEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseMove, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonDblClick, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseReleaseEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonRelease, event);
}

// ---------------------------------------------------------------------
void Opengl2::mouseWheelEvent(QWheelEvent *event)
{
  wheelEvent(event);
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
  int key1=0;
  int key=event->key();
  if (ismodifier(key)) return;
  if ((key==Qt::Key_unknown)||((key>=Qt::Key_F1)&&(key<=Qt::Key_F35))) {
    QWidget::keyPressEvent(event);
    return;
  } else
    key1=translateqkey(key);
  // Ctrl+anything becomes (possibly) a _fkey event; others become _char
  if (!event->modifiers().testFlag(Qt::ControlModifier)) {
    char sysdata[20];
    QString keyt = event->text();
    if (key==key1)
      sprintf(sysdata, "%s", event->text().toUtf8().constData());
    else sprintf(sysdata, "%s", QString(QChar(key1)).toUtf8().constData());

    pchild->event=std::string("char");
    pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
    pchild->sysdata=std::string(sysdata);
    pchild->pform->signalevent(pchild);
    // for ESC key, abort further processing lest we generate a second J event.
    if (key==Qt::Key_Escape) return;
  }
  QWidget::keyPressEvent(event);
}
