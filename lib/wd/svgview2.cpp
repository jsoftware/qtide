
#include <QApplication>
#include <QPainter>
#include <QImage>

#include "form.h"
#include "svgview2.h"
#include "svgview.h"
#include "pane.h"
#include "../base/state.h"

extern Form *form;

// ---------------------------------------------------------------------
SvgView2::SvgView2(Child *c, QWidget *parent) : QWidget(parent)
{
  Q_UNUSED(parent);
  pchild = c;
  type=pchild->type;
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setMouseTracking(true);           // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event

  m_renderer = 0;
  m_origin = QPoint(0,0);
  m_mouseDown = false;

  m_zoomSlider = new QSlider(Qt::Vertical, this);
  m_zoomSlider->setMaximum(300);
  m_zoomSlider->setMinimum(1);
  showZoom(false);

  connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));
  m_zoomSlider->setValue(100);

}

// ---------------------------------------------------------------------
SvgView2::~SvgView2()
{

}

// ---------------------------------------------------------------------
void SvgView2::showZoom(bool v)
{
  m_zoomSlider->setVisible(m_showzoom = v);
}

// ---------------------------------------------------------------------
void SvgView2::setFile(const QString &filePath)
{
  if(!m_renderer)
    m_renderer = new QSvgRenderer(filePath);
  else
    m_renderer->load(filePath);

  m_imageSize = m_renderer->viewBox().size();

  m_viewBoxCenter = (QPointF(m_imageSize.width() / qreal(2.0), m_imageSize.height() / qreal(2.0)));

  if (m_renderer->animated())
    connect(m_renderer, SIGNAL(repaintNeeded()), this, SLOT(update()));
  else
    disconnect(m_renderer, SIGNAL(repaintNeeded()), 0, 0);

}

// ---------------------------------------------------------------------
void SvgView2::setXml(const string & v)
{
  if(!m_renderer)
    m_renderer = new QSvgRenderer(QByteArray::fromRawData(v.c_str(),(int)v.length()));
  else
    m_renderer->load(QByteArray::fromRawData(v.c_str(),(int)v.length()));

  m_imageSize = m_renderer->viewBox().size();

  m_viewBoxCenter = (QPointF(m_imageSize.width() / qreal(2.0), m_imageSize.height() / qreal(2.0)));

  if (m_renderer->animated())
    connect(m_renderer, SIGNAL(repaintNeeded()), this, SLOT(update()));
  else
    disconnect(m_renderer, SIGNAL(repaintNeeded()), 0, 0);

}

// ---------------------------------------------------------------------
void SvgView2::printSVG()
{
  if (!m_renderer || !m_renderer->isValid()) return;
  QPainter p;
  p.begin(config.Printer);
  QRect rect = p.viewport();
  QSize size = m_renderer->viewBox().size();
  size.scale(rect.size(), Qt::KeepAspectRatio);
  p.setViewport(rect.x(), rect.y(), size.width(), size.height());
  m_renderer->render(&p);
  p.end();
}

// ---------------------------------------------------------------------
void SvgView2::saveasSVG(const QString& filepath)
{
  if (!m_renderer || !m_renderer->isValid()) return;
  QSize size = m_renderer->viewBox().size();
  QImage image(size.width(), size.height(), QImage::Format_ARGB32);
  QPainter painter(&image);
  m_renderer->render(&painter);
  image.save(filepath);
}

// ---------------------------------------------------------------------
void SvgView2::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event)
  if (!m_renderer || !m_renderer->isValid()) return;
  QPainter painter(this);
  m_renderer->setViewBox(QRectF(m_viewBox.left()+m_origin.x(),
                                m_viewBox.top()+m_origin.y(),
                                m_viewBoxSize.width(),
                                m_viewBoxSize.height()));
  m_renderer->render(&painter);
}


QRectF SvgView2::getViewBox(QPointF viewBoxCenter)
{
  QRectF result;
  result.setLeft(viewBoxCenter.x() - (m_viewBoxSize.width() / 2));
  result.setTop(viewBoxCenter.y() - (m_viewBoxSize.height() / 2));
  result.setRight(viewBoxCenter.x() + (m_viewBoxSize.width() / 2));
  result.setBottom(viewBoxCenter.y() + (m_viewBoxSize.height() / 2));
  return result;
}

void SvgView2::updateImageScale()
{
  m_imageScale = qMax( (qreal)m_imageSize.width() / (qreal)width(),
                       (qreal)m_imageSize.height() / (qreal)height())*m_zoomLevel;

  m_viewBoxSize.setWidth((qreal)width() * m_imageScale);
  m_viewBoxSize.setHeight((qreal)height() * m_imageScale);
}


void SvgView2::resizeEvent ( QResizeEvent * event )
{
  Q_UNUSED(event);

  qreal origZoom = m_zoomLevel;

  // Get the new bounds:
  m_zoomLevel = 1.0;
  updateImageScale();
  m_viewBoxBounds = getViewBox(QPointF(m_imageSize.width() / 2.0, m_imageSize.height() / 2.0));

  m_zoomLevel = origZoom;
  updateImageScale();
  m_viewBox = getViewBox(m_viewBoxCenter);

  QRect sliderRect;
  sliderRect.setLeft(width() - m_zoomSlider->sizeHint().width());
  sliderRect.setRight(width());
  sliderRect.setTop(height()/4);
  sliderRect.setBottom(height() - (height()/4));
  m_zoomSlider->setGeometry(sliderRect);

  pchild->event="resize";
  pchild->pform->signalevent(pchild);
}


int SvgView2::getZoom()
{
  return m_zoomLevel * qreal(100);
}


void SvgView2::setZoom(int newZoom)
{
  newZoom = qMin(300,qMax(1,newZoom));
  m_zoomLevel = qreal(newZoom) / qreal(100);

  updateImageScale();
  m_viewBox = getViewBox(m_viewBoxCenter);

  update();
}


QPoint SvgView2::getOrigin()
{
  return m_origin;
}


void SvgView2::setOrigin(int x, int y)
{
  m_origin = QPoint(x,y);
  update();
}


// ---------------------------------------------------------------------
void SvgView2::buttonEvent(QEvent::Type type, QMouseEvent *event)
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

  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysdata[200];
  sprintf(sysdata , "%d %d %d %d %d %d %d %d %d %d %d %d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);

  pchild->event=evtname;
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void SvgView2::wheelEvent(QWheelEvent *event)
{
  char deltasign = ' ';
  int delta = event->delta() / 8;  // degree
  if (delta<0) {
    delta = -delta;
    deltasign = '_';
  }

  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysdata[200];
  sprintf(sysdata , "%d %d %d %d %d %d %d %d %d %d %d %c%d", event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)), (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)), (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, deltasign, delta);

  pchild->event=string("mwheel");
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void SvgView2::mousePressEvent(QMouseEvent *event)
{
  m_mouseDown = true;
  m_lastOrigin = m_origin;
  m_mouseLastPos = QPoint(event->x(),event->y());
  buttonEvent(QEvent::MouseButtonPress, event);
}

// ---------------------------------------------------------------------
void SvgView2::mouseMoveEvent(QMouseEvent *event)
{
  if (m_mouseDown) {
    m_origin = QPoint(m_lastOrigin.x()+m_mouseLastPos.x()-event->x(),
                      m_lastOrigin.y()+m_mouseLastPos.y()-event->y());
    update();
  }
  buttonEvent(QEvent::MouseMove, event);
}

// ---------------------------------------------------------------------
void SvgView2::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonDblClick, event);
}

// ---------------------------------------------------------------------
void SvgView2::mouseReleaseEvent(QMouseEvent *event)
{
  m_mouseDown = false;
  buttonEvent(QEvent::MouseButtonRelease, event);
}

// ---------------------------------------------------------------------
void SvgView2::focusInEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focus";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void SvgView2::focusOutEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focuslost";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void SvgView2::keyPressEvent(QKeyEvent *event)
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
    if (key==key1)
      sprintf(sysdata , "%s", event->text().toUtf8().constData());
    else sprintf(sysdata , "%s", QString(QChar(key1)).toUtf8().constData());

    pchild->event=string("char");
    pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
    pchild->sysdata=string(sysdata);
    pchild->pform->signalevent(pchild);
    // for ESC key, abort further processing lest we generate a second J event.
    if (key==Qt::Key_Escape) return;
  }
  QWidget::keyPressEvent(event);
}

