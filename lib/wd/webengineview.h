#ifndef WEBENGINEVIEW_H
#define WEBENGINEVIEW_H

#include <QUrl>
#include <QWebEngineView>
#include "child.h"

class Form;
class Pane;
class Qwebengineview;
class WebEngineViewCom;

// ---------------------------------------------------------------------
class WebEngineView : public Child
{
  Q_OBJECT

public:
  WebEngineView(string n, string s, Form *f, Pane *p);

  void cmd1(QList<QVariant> obj);
  string get(string p,string v);
  void set(string p,string v);
  string state();

  QUrl baseUrl;
  QString cb_name;
  QString cb_value;
  QString curl;
  WebEngineViewCom *qcom;

private slots:
  void addJavaScriptObject();
  void urlChanged (const QUrl & url);

private:
  void cmd_callback(QList<QVariant> obj);
};

// ---------------------------------------------------------------------
class WebEngineViewCom : public QObject
{
  Q_OBJECT

public:
  WebEngineViewCom(WebEngineView *c,QWidget *parent = 0);
  WebEngineView *pchild;

public slots:
  void cmd(QList<QVariant> obj);
};

// ---------------------------------------------------------------------
class Qwebengineview : public QWebEngineView
{
  Q_OBJECT

public:
  Qwebengineview(Child *c, QWidget *p=0);

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
