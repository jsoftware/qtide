#ifndef QUICKVIEW_H
#define QUICKVIEW_H

#include <QUrl>
#ifdef QT50
#include <QQuickView>
#else
#include <QDeclarativeView>
#endif
#include "form.h"

// ---------------------------------------------------------------------
#ifdef QT50
class QuickView : public QQuickView
#else
class QuickView : public QDeclarativeView
#endif
{
  Q_OBJECT

public:
  QuickView(string n, string s, int resizemode);

  QUrl sourceUrl;
  int resizeMode;

#ifdef QT50
protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *e);

private slots:

  void statusChanged(QQuickView::Status status);
  void sceneGraphError(QQuickWindow::SceneGraphError, const QString &message);
#else
private slots:

  void statusChanged(QDeclarativeView::Status status);
  void sceneResized (QSize size);
#endif

};

#endif
