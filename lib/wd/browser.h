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
  Browser(std::string n, std::string s, Form *f, Pane *p);

  void cmd(std::string p, std::string v);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

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
