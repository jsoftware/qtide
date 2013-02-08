#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QUrl>
#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Webview : public Child
{
  Q_OBJECT

public:
  Webview(string n, string s, Form *f, Pane *p);
  void set(string p);
  void setp(string p,string v);

  QUrl baseUrl;

private slots:

};

#endif
