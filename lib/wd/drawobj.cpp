
#include "drawobj.h"

extern "C" int glc_clear2 (void *p,int clear);

// ---------------------------------------------------------------------
Drawobj::Drawobj()
{
  m_height=m_width=0;
  painter=0;
  font=0;
  pixmap=0;
  glc_clear2 (this,0);
}

// ---------------------------------------------------------------------
Drawobj::~Drawobj()
{
  freepixmap();
}

// ---------------------------------------------------------------------
int Drawobj::height()
{
  return m_height;
}

// ---------------------------------------------------------------------
int Drawobj::width()
{
  return m_width;
}

// ---------------------------------------------------------------------
void Drawobj::fill(const int *p)
{
  QColor c(*(p), *(p + 1), *(p + 2), *(p + 3));
  if (pixmap)
    pixmap->fill(c);
  else
    painter->fillRect(0,0,width(),height(),c);
}

// ---------------------------------------------------------------------
QPixmap Drawobj::getpixmap()
{
  QPixmap m;
  if (pixmap)
    return pixmap->copy(0,0,width(),height());
  return m;
}

// ---------------------------------------------------------------------
void Drawobj::resize(int w, int h)
{
  if (pixmap) {
    if (painter) delete painter;
    QPixmap *p=new QPixmap(w,h);
    painter=new QPainter(p);
    painter->drawPixmap(QPoint(0,0),*pixmap);
    delete pixmap;
    pixmap=p;
  } else {
    pixmap=new QPixmap(w,h);
    painter=new QPainter(pixmap);
  }
  if (painter->isActive()) painter->setRenderHint(QPainter::Antialiasing, true);
  m_width=w;
  m_height=h;
}

// ---------------------------------------------------------------------
void Drawobj::freepixmap()
{
  if (pixmap) {
    if (painter) delete painter;
    painter=0;
    delete pixmap;
    pixmap=0;
  } else if (painter) {
    delete painter;
    painter=0;
  }
  m_height=m_width=0;
}

