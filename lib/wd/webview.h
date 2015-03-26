#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QUrl>
#include "child.h"

class Form;
class Pane;
class WebKitView;

// ---------------------------------------------------------------------
class WebView : public Child
{
  Q_OBJECT

public:
  WebView(string n, string s, Form *f, Pane *p);
  ~WebView();
  string get(string p,string v);
  void set(string p,string v);
  string state();

  QUrl baseUrl;
  QString curl;

private slots:

  void urlChanged ( const QUrl & url );

};

#endif
