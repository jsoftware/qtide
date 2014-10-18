#ifndef OPENGL2_H
#define OPENGL2_H

#ifdef QT54
#include <QOpenGLWidget>
#else
#include <QGLWidget>
#endif
//#include <QEvent>
#include <QPainter>

#include "font.h"
#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
#ifdef QT54
class Opengl2 : public QOpenGLWidget
#else
class Opengl2 : public QGLWidget
#endif
{
  Q_OBJECT

public:
#ifdef QT54
  Opengl2(Child *c, const QSurfaceFormat& format, QWidget *parent);
#else
  Opengl2(Child *c, const QGLFormat& format, QWidget *parent);
#endif
  ~Opengl2();

  void fill(const int *);
  QPixmap getpixmap();
  void paintend();

  QBrush brush;
  Font *font;
  QPen pen;
  QPen textpen;
  QColor color;
  QColor pencolor;
  QColor brushcolor;
  QColor textcolor;

  QPainter *painter;

  bool antialiased;
  bool transformed;

  int brushnull;
  int clipped, textx, texty, orgx, orgy;

protected:
  void paintGL();
  void resizeGL(int,int);
  void initializeGL();

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void mouseWheelEvent(QWheelEvent *event);

private slots:

private:
  void buttonEvent(QEvent::Type type, QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  Child *pchild;
  bool initialized;

};

#endif
