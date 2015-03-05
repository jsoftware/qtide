#ifndef DUMMY_H
#define DUMMY_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Dummy : public Child
{
  Q_OBJECT

public:
  Dummy(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:

};

#endif
