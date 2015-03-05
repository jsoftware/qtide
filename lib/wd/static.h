#ifndef STATIC_H
#define STATIC_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Static : public Child
{
  Q_OBJECT

public:
  Static(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:

};

#endif
