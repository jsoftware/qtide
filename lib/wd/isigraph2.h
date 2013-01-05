#ifndef ISIGRAPH2_H
#define ISIGRAPH2_H

#include <string>
#include <QBrush>
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QRect>
#include <QWidget>
#include <QResizeEvent>
#include <QEvent>

#include "font.h"
#include "child.h"

class Isigraph2 : public QWidget
{
  Q_OBJECT

public:
  Isigraph2(Child *c);
  ~Isigraph2();

  QBrush brush;
  Font *font;
  QPen pen;
  QPen textpen;
  QColor color;
  QColor pencolor;
  QColor brushcolor;
  QColor textcolor;

  QPainter *painter;
  QPainterPath path;

  QPixmap *pixmap;

  bool antialiased;
  bool transformed;
  bool active;
  bool nopaint;
  bool jpaint;
  bool epaint;

  int brushnull;
  int clipped, textx, texty, orgx, orgy;

protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent (QResizeEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void mouseWheelEvent(QWheelEvent *event);

private:
  void buttonEvent (QEvent::Type type, QMouseEvent *event);
  void wheelEvent (QWheelEvent *event);
  Child *pchild;

};

#endif

