#ifndef WEBENGINE_H
#define WEBENGINE_H

#include <QUrl>
#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Webengine : public Child
{
  Q_OBJECT

public:
  Webengine(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
  string state();

  QUrl baseUrl;
  QString curl;

private slots:

  void urlChanged ( const QUrl & url );

};

#endif
