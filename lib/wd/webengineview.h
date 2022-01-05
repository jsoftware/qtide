#ifndef WEBENGINEVIEW_H
#define WEBENGINEVIEW_H

#include <QUrl>
#include <QWebEnginePage>
#include <QWebEngineView>
#include "child.h"

class Form;
class Pane;
class Qwebenginepage;
class Qwebengineview;
class WebEngineView;
class WebEngineViewCom;

// ---------------------------------------------------------------------
class WebEngineView : public Child
{
  Q_OBJECT

public:
  WebEngineView(std::string n, std::string s, Form *f, Pane *p);
  ~WebEngineView();

  void cmd(std::string p, std::string v);
  void cmdpost(QStringList obj);
  void set(std::string p,std::string v);
  std::string state();

  QUrl baseUrl;
  QString cb_name;
  QString cb_value;
  QString curl;
  bool loadok;
  WebEngineViewCom *qcom;

private slots:
  void urlChanged (const QUrl & url);
  void loadFinished(bool);
};

// ---------------------------------------------------------------------
class WebEngineViewCom : public QObject
{
  Q_OBJECT

public:
  WebEngineViewCom(WebEngineView *c,QWidget *parent = 0);
  WebEngineView *pchild;

  void qwrite(QStringList);

  QStringList qcomval;

signals:
// qt to html
  void qNotify(QStringList);

public slots:
// html to qt
  void qPost(QStringList);
};

// ---------------------------------------------------------------------
class Qwebenginepage : public QWebEnginePage
{
  Q_OBJECT

public:
  Qwebenginepage();

protected:
  bool acceptNavigationRequest(const QUrl &, QWebEnginePage::NavigationType, bool);

private:
  Child *pchild;
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
