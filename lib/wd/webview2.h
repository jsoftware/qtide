#ifndef WEBVIEW2_H
#define WEBVIEW2_H

#include <QUrl>
#include "child.h"

class Form;
class Pane;
class Webengineview;

// ---------------------------------------------------------------------
class Webview2 : public Child
{
  Q_OBJECT

public:
  Webview2(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
  string state();

  QUrl baseUrl;
  QString curl;

private slots:

  void urlChanged ( const QUrl & url );

};

#endif
