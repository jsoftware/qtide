#ifndef QUICKVIEW2_H
#define QUICKVIEW2_H

#include <QUrl>
#include <QQuickView>
#include "form.h"

// ---------------------------------------------------------------------
class QuickView2 : public QQuickView
{
  Q_OBJECT

public:
  QuickView2(string n, string s, int resizemode, string glver);

  QUrl sourceUrl;
  int resizeMode;

protected:
  void keyPressEvent(QKeyEvent *event);

private slots:

  void statusChanged(QQuickView::Status status);
#ifdef QT53
  void sceneGraphError(QQuickWindow::SceneGraphError, const QString &message);
#endif
  void closeview ();

};

#endif
