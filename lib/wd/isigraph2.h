#ifndef ISIGRAPH2_H
#define ISIGRAPH2_H

#include <string>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPixmap>

#include "font.h"
#include "child.h"

class Isigraph2 : public QWidget
{
  Q_OBJECT

public:
  Isigraph2(Child *c, QWidget *parent = 0);
  virtual ~Isigraph2();

  void fill(const int *);
  QPixmap getpixmap();
  void paintend();
  std::string type;

  QBrush brush;
  Font *font;
  QPen pen;
  QPen textpen;
  QPixmap *pixmap;

  QColor color;
  QColor pencolor;
  QColor brushcolor;
  QColor textcolor;

  QPainter *painter;
  QPainterPath path;

  bool antialiased;
  bool ifpixmap;
  bool transformed;

  int brushnull;
  int clipped, textx, texty, orgx, orgy;

protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void keyPressEvent(QKeyEvent *event);

private slots:

private:
  void buttonEvent(QEvent::Type type, QMouseEvent *event);
  void paintEvent_isidraw();
  void paintEvent_isigraph();
  void wheelEvent(QWheelEvent *event);
  Child *pchild;

};

#endif
