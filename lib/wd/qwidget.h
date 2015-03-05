#ifndef QWIDGEX_H
#define QWIDGEX_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class QWidgex : public Child
{
  Q_OBJECT

public:
  QWidgex(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:

};

#endif
