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
  QuickView2(std::string n, std::string s, int resizemode, std::string glver);

  QUrl sourceUrl;
  int resizeMode;

protected:
  void keyPressEvent(QKeyEvent *event);

private slots:

  void statusChanged(QQuickView::Status status);
  void sceneGraphError(QQuickWindow::SceneGraphError, const QString &message);
  void closeview ();

};

#endif
