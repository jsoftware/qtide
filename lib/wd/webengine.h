#ifndef WEBENGINE_H
#define WEBENGINE_H

#include <QUrl>
#include "child.h"

class Form;
class Pane;
class WebEngineView;

// ---------------------------------------------------------------------
class WebEngine : public Child
{
  Q_OBJECT

public:
  WebEngine(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

  QUrl baseUrl;
  QString curl;

private slots:

  void urlChanged ( const QUrl & url );

};

#endif
