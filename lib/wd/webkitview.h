#ifndef WEBKITVIEW_H
#define WEBKITVIEW_H

#include <QWebView>

#include "child.h"

// ---------------------------------------------------------------------
class WebKitView : public QWebView
{
  Q_OBJECT

public:
  WebKitView (Child *c, QWidget *parent = 0);

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  void buttonEvent(QEvent::Type type, QMouseEvent *event);
  Child *pchild;

};

#endif
