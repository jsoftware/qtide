#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QUrl>
#include "child.h"

class Form;
class Pane;
class Webkitview;

// ---------------------------------------------------------------------
class Webview : public Child
{
  Q_OBJECT

public:
  Webview(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
  string state();

  QUrl baseUrl;
  QString curl;

private slots:

  void urlChanged ( const QUrl & url );

};

#endif
