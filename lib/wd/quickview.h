#ifndef QUICKVIEW_H
#define QUICKVIEW_H

#include <QUrl>
#include <QQuickView>
#include "form.h"

// ---------------------------------------------------------------------
class QuickView : public QQuickView
{
  Q_OBJECT

public:
  QuickView(string n, string s, int resizemode);

  QUrl sourceUrl;
  int resizeMode;

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);

private slots:

  void statusChanged(QQuickView::Status status);
  void sceneGraphError(QQuickWindow::SceneGraphError, const QString &message);

};

#endif
