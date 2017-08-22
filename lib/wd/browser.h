#ifndef BROWSER_H
#define BROWSER_H

#include <QUrl>
#include <QTextBrowser>
#include "child.h"

class Form;
class Pane;
class Qbrowser;
class QPrinter;

// ---------------------------------------------------------------------
class Browser : public Child
{
  Q_OBJECT

public:
  Browser(string n, string s, Form *f, Pane *p);

  void cmd(string p, string v);
  string get(string p,string v);
  void set(string p,string v);
  string state();

  QUrl baseUrl;

#ifndef QT_NO_PRINTER
public slots:
  void printPreview(QPrinter * printer);
#endif

private slots:
  void sourceChanged (const QUrl & url);

};

// ---------------------------------------------------------------------
class Qbrowser : public QTextBrowser
{
  Q_OBJECT

public:
  Qbrowser(Child *c, QWidget *p=0);

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  void buttonEvent(QEvent::Type type, QMouseEvent *event);
  Child *pchild;
};

#endif
