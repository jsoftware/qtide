#ifndef QUICKVIEW1_H
#define QUICKVIEW1_H

#include <QUrl>
#ifdef QTFALSE
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeItem>
#else
#include <QDeclarativeView>
#endif
#include "form.h"

// ---------------------------------------------------------------------
#ifdef QTFALSE
class QuickView1 : public QDeclarativeView
#else
class QuickView1 : public QDeclarativeView
#endif
{
  Q_OBJECT

public:
  QuickView1(string n, string s, int resizemode);

  QUrl sourceUrl;
  int resizeMode;

#ifdef QTFALSE
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
  void closeview ();

};

#endif
