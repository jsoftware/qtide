#ifndef QUICKWIDGET_H
#define QUICKWIDGET_H

#include <QUrl>
#include <QQuickWidget>
#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class QuickWidget : public Child
{
  Q_OBJECT

public:
  QuickWidget(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

  QUrl sourceUrl;
  int resizeMode;

private slots:

  void statusChanged(QQuickWidget::Status status);
  void sceneGraphError(QQuickWindow::SceneGraphError, const QString &message);

};

#endif
