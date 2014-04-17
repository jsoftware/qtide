#ifndef QUICKVIEW2_H
#define QUICKVIEW2_H

#include <QUrl>
#ifdef QT50
#include <QQuickView>
#else
#include <QDeclarativeView>
#endif
#include "form.h"

// ---------------------------------------------------------------------
#ifdef QT50
class QuickView2 : public QQuickView
#else
class QuickView2 : public QDeclarativeView
#endif
{
  Q_OBJECT

public:
  QuickView2(string n, string s, int resizemode);

  QUrl sourceUrl;
  int resizeMode;

#ifdef QT50
protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);

private slots:

  void statusChanged(QQuickView::Status status);
#ifdef QT53
  void sceneGraphError(QQuickWindow::SceneGraphError, const QString &message);
#endif
#else
private slots:

  void statusChanged(QDeclarativeView::Status status);
  void sceneResized (QSize size);
#endif
  void closeview ();

};

#endif
