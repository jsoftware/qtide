#ifndef OPENGL2_H
#define OPENGL2_H

#include <QGLWidget>
#include <QEvent>
#include <QTimer>

// #include <GLES2/gl2.h>
// #include <GLES2/gl2ext.h>

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class Opengl2 : public QGLWidget
{
  Q_OBJECT

public:
  Opengl2(Child *c);
  ~Opengl2();
  void paintgl ();

  bool epaint;
  bool jpaint;
  bool nopaint;

public slots:

protected:
  void paintGL ();
  void resizeGL (int,int);
  void initializeGL ();

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void mouseWheelEvent(QWheelEvent *event);

private slots:
  void timeout();

private:
  void buttonEvent (QEvent::Type type, QMouseEvent *event);
  void wheelEvent (QWheelEvent *event);
  Child *pchild;
  bool initialized;

  QTimer *timer;

};

#endif
